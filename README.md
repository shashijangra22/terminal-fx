Name: Shashi Jangra
Email ID: shashi.jangra@students.iiit.ac.in
Roll Number: 2018202001
M.Tech CSIS

A terminal based file explorer that operates in 2 modes:

	Normal Mode: 
	
	The program starts in normal mode and lists the files in directory where it is started.(That becomes the root/home address)
	On pressing:
	
	Enter: Open a directory in normal mode
			Open a file in its default application
	
	BackSpace key: Go up one level in the explorer but not beyond the root path
	'h' key: Go to Home Directory
	
	Left Arrow: Go back to the previously visited directory if its possible
	
	Right Arrow: Go forward to the directory where user went by pressing back key
	
	Up/Down Arrow: move the cursor to navigate in the explorer (By default the program displays 10 files, if there 	are more than 		10 files than user can view by scrolling with these keys)

	'q': It quits the program

	':': Switch to command mode

User can switch to Command Mode with the ':' key and perform following commands:
	
	Assumptions/Guidelines:	

		The source file/path should be always given as full name of file/directory without any '/' character
		for example your directory structure is ~/abc/xyz/def/foo.txt and your program started in abc then abc becomes root directory for the program lifetime and thus,
		The destination path can take two kind of values:
			Absolute Path(Path from the root/home): It must begin with '~/' for example ~/abc/xyz/def/foo.txt
			Relative Path(Path from the current directory): It must begin with '/' for example if you are in xyz then /def/foo.txt
		Some Commands like create_file supports '.' as destination path
	Command Mode: 

	Copy File(s):			copy <source_file(s)> <destination_directory>

	Create File(s):			create_file <source_file(s)> <destination_directory>
	Create Directory(s):	create_file <source_file(s)> <destination_directory>
	Move File(s):			move <source_file(s)> <destination_directory>
	Rename File(s):			rename <old_filename> <new_filename>
	Delete File(s): 		delete_file <file_path>
	Copy Directory(s): 		copy_dir <source_file(s)> <destination_directory>
	Move Directory(s): 		move_dir <source_file(s)> <destination_directory>
	Delete Directory(s): 	del_dir <source_file(s)> <destination_directory>
	Search Files:			search <source_file>
		All files in the working tree will be displayed on the same screen
	Goto Directory:			goto <directory_path>
		program will come in normal mode to explore the directory
	Snapshot:				snapshot <directory_path> <dumpfile>
	exit:	program will come to normal mode