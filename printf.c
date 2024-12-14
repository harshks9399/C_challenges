// not even including standard C library
// the very basic syscall lib included
#include<unistd.h>
// used 'write' function
// the ARGS marco 


//defining fucntion macro
#define putchar(x) write(1, chardup(x), 1);

#define Wait4char   1 // 0001
#define Wait4fmt    2 // 0010 

typedef unsigned char State;


// args function ?
/*
** Args :: Int* -> Int*
*/
#define Args(arg) \
    unsigned int *_argp; \
    __asm__("movl %%ebp, %0" : "=r"(_argp)); \
    arg = (unsigned int *)(_argp + 2);

// #define Args(arg) \
//     unsigned int _x; \
//     __asm__("mov %%ebp,%0":"=r"(_x)); \
//     _x += 8; \
//     arg = (unsigned int *)_x;


// the above macro is used to manipulate stack pointer to access function arguements
//inline_assembly (__asm)
//to move the value of the base pointer register(ebp) into _x
//to handle variable length arguements



char *chardup(const char c){
    // to convert char to string
    static char buffer[2]; //static-> so that we dont reallocate memeory to buffer again and again
    char * p;
    p = buffer;
    *p++ = c;
    *p = 0;
    return buffer;
}

unsigned int strlen(const char * s){
    // to find the string length
    unsigned int n;
    const char *p;
    for(p=s, n=0; *p ; p++, n++);
    return n;
}

int puts(const char *s){
    // to print a line/string
    unsigned int n = strlen(s);
    if(n < 1) return -1; // error condition
    return write(1, s, n);
}


char * itoa(int n){ // integer to array
    // reverse the number 
    // the get digit by digit in reverse order of the reversed number
    // simple base case for n = 0;
    static char array_num[12];
    int rev_n = 0;
    if(n==0){
        array_num[0] = '0';
        array_num[1] = '\0';
        return array_num;
    }
    int neg = 0;
    if(n < 0){
        neg = 1;
        n *= -1;
    }
    int len = 0;
    while(n!=0){
        int x = n%10;
        n /= 10;
        rev_n = 10*rev_n + x;
        len++;
    }
    if(neg) len++;
    // char *array_num = (char *)malloc(len + 1); // -> this would require the use of stdlib, which we dont want
    
    int idx = 0;
    if(neg) array_num[idx++] = '-';
    while(rev_n){
        int x = rev_n % 10;
        rev_n /= 10;
        array_num[idx++] = (char)((int)('0') + x);
    }
    array_num[idx] = '\0';
    return array_num;
}


// variable number of arguements, thats why the 3 dots
int printf(const char *fmt, ...) {
    unsigned int *p;
    const char *f;
    State s;
    char * temp ;
    s = Wait4char;
    Args(p); // Initialize pointer to first argument i.e. fmt
    p++;
    f = fmt;

    do {
        if (s & Wait4char) {
            switch (*f) {
                case '%': // Switch to Wait4fmt on '%'
                    s = Wait4fmt;
                    break;

                default: // Print character as is
                    putchar(*f);
            }
        } else if (s & Wait4fmt) {
            switch (*f) {
                case '%': // Literal '%'
                    putchar(*f);
                    s = Wait4char;
                    break;

                case 's': // String argument
                    puts((unsigned char *)*p);
                    p++; // Move to the next argument
                    s = Wait4char;
                    break;

                case 'c': // char argument
                    putchar((char)*p);
                    p++;
                    s = Wait4char;
                    break;
                
                case 'd': // int arguemnt
                    // const char * temp ; // this cannot be declared inside this case scope unless we make a block inside the case statement
                    temp = itoa((int)*p);
                    p++;
                    puts(temp);
                    s = Wait4char;
                    break;

                default: // Unsupported specifier
                    s = Wait4char;
                    break;
            }
        }
    } while (*(f++));

    return 0;
}





int main(){
    char * p = "Checkingl";
    const char c = 'a';
    int num1 = 0;
    printf("Hello world %d %s abc\n %s, %c\n\n", num1, p, p, c);
        
    int num = 12345;
    puts(itoa(num));
    return 0;


}



