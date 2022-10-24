# Vigenère's Text Editor

A command-line operable text editor, built with [C](https://en.wikipedia.org/wiki/C_(programming_language)), made for 
quick and easy editing of files and having the additional features of being able to encrypt/decrypt files using the [Vigenere cipher](https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher) and being able to
display the number of words/characters/letters/sentences in a file.

## How to use:
### Compile to executable:

- Install [C-Program Compiler (gcc)](https://gcc.gnu.org/)
- Run `gcc -o VTE VTE.c`



### Input: 
Command-line arguments, the first being the name of the operation and second the name of the file being operated on.

The program only performs 1 operation per run(easier to come back to the previous command in terminal rather than inputting the entire thing again in stdin) 
with the first command-line argument being the name of the operation performed, inputted exactly as one of the following:

- `CREATE` - Create a new file with a specified name.
- `COPY` - Create a new file with a specified name and identical contents to an existing file.
- `DELETE` - Delete an existing file with a specified name.
- `DISPLAY` - Display the contents of an existing file with a specified name.
- `APPEND` - Create a new line of content at the end of a specified file.
- `EXTERMINATE` - Delete a line of content at a particular line number in a specified file.
- `INSERT` - Create a new line of content at a particular line number in a specified file.
- `SHOW` - Display the contents of a file at a particular line number in a specified file.
- `LOG` - Display the sequence of operations performed on all files created by the program, including the number of lines following each operation and the individual line worked with.This operation does not require a file name.
- `LINES` - Show the number of lines in a specified file.
- `ENCRYPT` - Encrypts the contents of an existing file with specified name using the Vigenere cipher and a
key in-putted by the user as a 3rd command-line argument.
- `DECRYPT` - Decrypts the contents of an existing file with specified name using the Vigenere cipher and a key that was used to encrypt the file in-putted by the user as a 3rd command-line argument.
- `COUNT` - Shows the number of X where X is one of the following: words,characters,letters or sentences and X is in-putted exactly as listed above as a 3rd command-line argument.

The second command-line argument is the name of the file to perform this operation on. 
For example:  
- `./VTE CREATE file.txt` if we want to create a file.txt.
- `./VTE ENCRYPT file.txt TSwift` if we want to encrypt file.txt using the key TSwift.
- `./VTE COUNT file.txt words` if we want count the number of words in file.txt.
- `./VTE LOG` if we want to display the change log of all your actions in current directory.

Note that the file should be in the same directory as the executable.


### ChangeLog.txt:
At the end of every operation a text file named ”ChangeLog.txt” will be opened(or created) and a record of the operation, 
the line worked with and the line count will be appended to the file. The operation `LOG` simply displays ”ChangeLog.txt” on standard output using the DISPLAY operation but you can always just open the file externally and have a look.

### Encryption and Decryption with Vigenère cipher
The security of this cipher depends a lot on the key the user chooses, particularly, the key length and how random it is. 
A key that would be very good would be as random as it can be and have the length of 
the text you want to encrypt(as it's the cipher text to key length ratio that matters),
that way it would be virtually impossible to crack. On the other hand if your key is 
short(and text is large) and is in the dictionary(not random) it would be very easily 
cracked. My implementation maintains the case of the characters and also maintains any 
punctuation(including spaces). Although it would make the cipher a bit easier to break 
it would only be so if the cipher is already more or less easy to break, therefore, given the 
strong key it would be better to fully retain the original text after decryption. 
As chars in C are represented with their ASCII value I’ve used that to index the 
alphanumeric letters for my cipher however as its only alphanumeric letters that are 
indexed my implementation would only encrypt plain English text(other characters just 
won't get encrypted) and the key should only consist of alphanumeric letters.





## Max bytes per line:
For the maximum number of bytes my program can process per line I’ve put 320.This is because the standard line width is considered to be up to 80 characters however some characters can take up to 4 bytes so I allocated enough memory to be able to process 80 4 byte characters. Although I could have put much more it would be poor memory management in my opinion as nearly no one puts more than 80 characters per line so making the program slower 100% of the time just to make it a little bit more convenient 1% of the time(if not less) is not optimized.