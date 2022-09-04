<h1 align="center">Payment Application</h1>

This is a simple payment application done as a project for the [egFWD](https://egfwd.com/) track [Professional Embedded Systems](https://egfwd.com/specializtion/professional-embedded-systems/) in partnership with [Udacity](https://www.udacity.com/) and [Sprints](https://sprints.ai/).

## Directory Structure

1. [`code`](code/): Contains the source code for the application.
2. [`screenshots`](screenshots/): Contains the screenshots of the application header files of the application.
3. [`recordings`](recordings/): Contains the recordings of testing the application.

## How to run the application

**To run unit testing**:

1. In [code\Application\appTest.c](code/Application/appTest.c), uncomment only one line of the required test in main function
2. Open the [`code`](code/) directory in command line
3. Run this command ```gcc Application\appTest.c Card\card.c Server\server.cTerminal\terminal.c -Wall -Werror```
4. Then run this command ```a.exe```
5. Repeat from step 1 to test other units

**To run application**:

1. Open the [`code`](code/) directory in command line
2. Run this command: ```gcc Application\app.c Application\state.c Card\card.c Server\server.c Terminal\terminal.c -Wall -Werror```
3. Then run this command ```a.exe```
