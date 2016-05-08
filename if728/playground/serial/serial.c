#include <REG517A.H>

#define XTAL 12000000
#define TBUF_SIZE 16
#define RBUF_SIZE 16

static unsigned char tbuf[TBUF_SIZE];
static unsigned char rbuf[RBUF_SIZE];

static unsigned char t_in = 0;
static unsigned char t_out = 0;
static unsigned char t_disabled = 0;

static unsigned char r_in = 0;
static unsigned char r_out = 0;

static void isr() interrupt 4 using 2 {
  /* Received data interrupt */
  if (RI0 != 0) {
    RI0 = 0;
    if ((r_in + 1) != r_out) {
      rbuf[r_in++%RBUF_SIZE] = S0BUF;
    }
  }

  /* Transmitted data interrupt*/
  if (TI0 != 0) {
    TI0 = 0;
    if (t_in != t_out) {
      S0BUF = tbuf[t_out++%TBUF_SIZE];
    }else {
      t_disabled = 1;
    }
  }

}

unsigned char rbuflen() {
  return (r_in - r_out);
}

unsigned char tbuflen() {
  return (t_in - t_out);
}

void set_baudrate(unsigned baudrate) {
  EAL = 0;

  /* Clear transmit interrupt and buffer */
  TI0 = 0;                          /* clear transmit interrupt */
  t_in = 0;                         /* empty transmit buffer */
  t_out = 0;
  t_disabled = 1;                   /* disable transmitter */

  /* Set timer 1 up as a baud rate generator */
  TR1 = 0;                          /* stop timer 1 */
  ET1 = 0;                          /* disable timer 1 interrupt */
  PCON |= 0x80;                     /* 0x80=SMOD: set serial baudrate doubler */
  TMOD &= ~0xF0;                    /* clear timer 1 mode bits */
  TMOD |= 0x20;                     /* put timer 1 into MODE 2 */
  TH1 = (unsigned char)
        (256 - (XTAL / (16L * 12L * baudrate)));
  TR1 = 1;                          /* start timer 1 */
  EAL = 1;                          /* enable interrupts */
}

void initialize(unsigned baudrate) {
  /* Setup TIMER1 to generate the proper baud rate */
  set_baudrate(baudrate);

  /* Clear com buffer indexes */
  EAL = 0;                          /* disable interrupts */
  t_in = 0;
  t_out = 0;
  t_disabled = 1;
  r_in = 0;
  r_out = 0;

  /* Setup serial port registers */
  SM0 = 0;
  SM1 = 1;                          /* serial port MODE 1 */
  SM20 = 0;
  REN0 = 1;                         /* enable serial receiver */
  TI0 = 0;                          /* clear transmit interrupt */
  RI0 = 0;                          /* clear receiver interrupt */
  ES0 = 1;                          /* enable serial interrupts */
  EAL = 1;                          /* enable interrupts */
}

int _putChar(char ch) {
  /* If the buffer is full, return an error value */
  if ((TBUF_SIZE - tbuflen()) <= 2) {
    return -1;
  }

  /* Add the data to the transmit buffer.  If the
  transmit interrupt is disabled, then enable it */
  EAL = 0;                          /* disable interrupts */
  tbuf[t_in++%TBUF_SIZE] = ch;

  if (t_disabled) {
    t_disabled = 0;
    TI0 = 1;                        /* enable it */
  }

  EAL = 1;                          /* enable interrupts */
  return 0;
}

void sendChar(char ch) {
  int i;
  while (_putChar(ch) != 0) {
    for (i=0; i<1000; i++);
  }
}

void sendString(char *str) {
  sendChar(*str);
  while(*(str++) != '\0') {
    sendChar(*str);
  }
}

char _getChar() {
  char ch;

  if (rbuflen() == 0) {
    return (-1);
  }

  EAL = 0;                          /* disable interrupts */
  ch = rbuf[r_out++%RBUF_SIZE];
  EAL = 1;                          /* enable interrupts */

  return ch;
}

char receiveChar() {
  char k;
  while((k = _getChar()) == -1);
  return k;
}

void receiveString(char *str) {
  while((*(str++) = receiveChar()) != '\r');
}

void main() {
  initialize(9600);        /* initialize interrupt driven serial I/O setup for 38400 baud */
  while(1) {
    char str[16] = {0};
    sendString("Hello World! Tell me something:\n");
    receiveString(str);
    sendString("You told me: ");
    sendString(str);
    sendChar('\n');
  }
}
