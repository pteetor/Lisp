//
// ConsoleBuffer - Manage user input
//
class ConsoleBuffer {
  static const int MAX_CHARS = 255;

  char buffer[MAX_CHARS];
  char* pBuf;
  int nChar;
  bool bEOF;
  bool bInteractive;

  bool refill();
  void prompt();
  
 public:
  ConsoleBuffer();
  
  char ahead();
  bool more();   // True if non-white-space remains on line
  char next();
  char now();
  bool eof() const;
};
