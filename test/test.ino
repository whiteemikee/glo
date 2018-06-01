#define ledPin 13
#define DOT 100
#define DASH 500
#define REST 200

void onFor(int duration) {
  if (duration < 1) return;
  digitalWrite(ledPin, HIGH);
  delay(duration);
  digitalWrite(ledPin, LOW);
}

void doDot() {
  onFor(DOT);
  delay(REST);
}

void doDash() {
  onFor(DASH);
  delay(REST);
}

void on(const char* dotDash) {
  unsigned int i;
  for (i=0; i<strlen(dotDash); i++) {
    if (dotDash[i] == '.') doDot();
    else if(dotDash[i] == '-') doDash();
  }
}

void char2morse(char c) {
  switch (c) {
    case 'a':
    case 'A':
      on(".-");
      break;
    case 'b':
    case 'B':
      on("-...");
      break;
    case 'c':
    case 'C':
      on("-.-.");
      break;
    case 'd':
    case 'D':
      on("-..");
      break;
    case 'e':
    case 'E':
      on(".");
      break;
    case 'f':
    case 'F':
      on("..-.");
      break;
    case 'g':
    case 'G':
      on("--.");
      break;
    case 'h':
    case 'H':
      on("....");
      break;
    case 'i':
    case 'I':
      on("..");
      break;
    case 'j':
    case 'J':
      on(".---");
      break;
    case 'k':
    case 'K':
      on("-.-");
      break;
    case 'l':
    case 'L':
      on(".-..");
      break;
    case 'm':
    case 'M':
      on("--");
      break;
    case 'n':
    case 'N':
      on("-.");
      break;
    case 'o':
    case 'O':
      on("---");
      break;
    case 'p':
    case 'P':
      on(".--.");
      break;
    case 'q':
    case 'Q':
      on("--.-");
      break;
    case 'r':
    case 'R':
      on(".-.");
      break;
    case 's':
    case 'S':
      on("...");
      break;
    case 't':
    case 'T':
      on("-");
      break;
    case 'u':
    case 'U':
      on("..-");
      break;
    case 'v':
    case 'V':
      on("...-");
      break;
    case 'w':
    case 'W':
      on(".--");
      break;
    case 'x':
    case 'X':
      on("-..-");
      break;
    case 'y':
    case 'Y':
      on("-.--");
      break;
    case 'z':
    case 'Z':
      on("--..");
      break;
    default:
      break;
  }
  delay(REST * 2);
}

void str2morse(const char* str) {
  unsigned int i;
  for(i = 0; i < strlen(str); i++) {
    char2morse(str[i]);
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
}

const char* msg = "boop";
void loop() {
  str2morse(msg);
}
