/* Assignment 1
   Moses Yang
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* States of DFA */
enum Statetype {NORMAL, FORWARD_SLASH, IN_COMMENT, ASTERISK, CHAR_LIT,
   STRING_LIT, ESCAPE_STRING, ESCAPE_CHAR};

/* Tracks line number, always starts at line 1 */
int line = 1;
/* Tracks line number for unterminated comment */
int commentLine = 0;


/* Function that handles NORMAL state. Parameter, int c, is a character
   read in from stdin that is analyzed. Returns a new state based on 
   the input argument */
enum Statetype handleNormalState(int c) {
   enum Statetype state;
   if(c == '/') 
      state = FORWARD_SLASH;
   else if(c == '\'') {
      state = CHAR_LIT;
      putchar(c);
   }
   else if(c == '\"') {
      state = STRING_LIT;
      putchar(c);
   }
   else {
      state = NORMAL;
      putchar(c);
   }
   return state;
}

/* Function that handles FORWARD_SLASH state. Parameter, int c, is a 
   character read in from stdin that is analyzed. Returns a new state 
   based on the input argument */
enum Statetype handleForwardSlashState(int c) {
   enum Statetype state;
   if(c == '*') {
      state = IN_COMMENT;
      putchar(' ');
      commentLine = line;
   }
   else if(c == '/') {
      state = FORWARD_SLASH;
      putchar('/');
   }
   else if(c == '"') {
      state = STRING_LIT;
      putchar('/');
      putchar(c);
   }
   else if(c == '\'') {
      state = CHAR_LIT;
      putchar('/');
      putchar(c);
   }
   else {
      state = NORMAL;
      putchar('/');
      putchar(c);
   }
   return state;
}

/* Function that handles IN_COMMENT  state. Parameter, int c, is a 
   character read in from stdin that is analyzed. Returns a new state 
   based on the input argument */
enum Statetype handleInCommentState(int c) {
   enum Statetype state;
   if(c == '*') {
      state = ASTERISK;
   }
   else if(c == '\n') {
      state = IN_COMMENT;
      putchar('\n');
   }
   else
      state = IN_COMMENT;
   return state;
}
/* Function that handles ASTERISK state. Parameter, int c, is a 
   character read in from stdin. Returns a new state based on the 
   input argument*/
enum Statetype handleAsteriskState(int c) {
   enum Statetype state;
   if(c == '/') 
      state = NORMAL;
   else if(c == '*')
      state = ASTERISK;
   else if(c == '\n') {
      state = IN_COMMENT;
      putchar('\n');
   }
   else
      state = IN_COMMENT;
   return state;
}

/* Function that handles CHAR_LIT state. Parameter, int c, is a 
   character read in from stdin that is analyzed. Returns a new state 
   based on the input argument*/
enum Statetype handleCharLitState(int c) {
   enum Statetype state;
   if(c == '\'') {
      state = NORMAL;
      putchar(c);
   }
   else if(c == '\\') {
      state = ESCAPE_CHAR;
      putchar(c);
   }
   else {
      state = CHAR_LIT;
      putchar(c);
   }
   return state;
}

/* Function that handles STRING_LIT state. Parameter, int c, is a 
   character read in from stdin that is analyzed. Returns a new state
   based on the input argument */
enum Statetype handleStringLitState(int c) {
   enum Statetype state;
   if(c == '"') {
      state = NORMAL;
      putchar(c);
   }
   else if(c == '\\') {
      state = ESCAPE_STRING;
      putchar(c);
   }
   else {
      state = STRING_LIT;
      putchar(c);
   }
   return state;
}


/* Function that handles ESCAPE_STRING state. Parameter, int c, is a 
   character read in from stdin that is analyzed. Returns a STRING_LIT
   state */
enum Statetype handleEscapeStringState(int c) {
   if(c=='n') {
      putchar('\n');
   }
   else {
      putchar(c);
   }
   return STRING_LIT;
}


/* Function that handles ESCAPE_CHAR  state. Parameter, int c, is a 
   character read in from stdin that is analyzed. Returns a CHAR_LIT
   state */
enum Statetype handleEscapeCharState(int c) {
   if(c=='n') {
      putchar('\n');
   }
   else {
      putchar(c);
   }
   return CHAR_LIT;
}

       
int main(void) {
   /* int variable that will hold character from stdin */
   int c;
   enum Statetype state = NORMAL;

   /* Read from stdin */
   while((c = getchar()) != EOF) {
      if (c == '\n') { line++; }

      switch (state) {
         case NORMAL:
            state = handleNormalState(c);
            break;
         case FORWARD_SLASH:
            state = handleForwardSlashState(c);
            break;
         case IN_COMMENT:
            state = handleInCommentState(c);
            break;
         case ASTERISK:
            state = handleAsteriskState(c);
            break;
         case CHAR_LIT:
            state = handleCharLitState(c);
            break;
         case STRING_LIT:
            state = handleStringLitState(c);
            break;
         case ESCAPE_STRING:
            state = handleEscapeStringState(c);
            break;
         case ESCAPE_CHAR:
            state = handleEscapeCharState(c);
            break;
      }


   }

   /* Check edge case */
   if (state == FORWARD_SLASH)
      putchar('/');
 
   if(state == IN_COMMENT || state == ASTERISK) {
       fprintf(stderr, "Error: line %d: unterminated comment\n",
               commentLine);
       exit(EXIT_FAILURE);
      }
    return 0;
} /* end main */

