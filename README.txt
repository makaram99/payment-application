To run unit testing:
	1. In "code\Application\appTest.c", uncomment only one line of the required test in main function
	2. Open the "code" directory in command line
	3. Run this command "gcc Application\appTest.c Card\card.c Server\server.c Terminal\terminal.c -Wall -Werror"
	4. Then run this command "a.exe"
	5. Repeat from step 1 to test other units

To run application:
	1. Open the "code" directory in command line
	3. Run this command:
		"gcc Application\app.c Application\state.c Card\card.c Server\server.c Terminal\terminal.c -Wall -Werror"
	4. Then run this command "a.exe"


