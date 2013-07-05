vfs-README
-------------
Arpit Mittal



RUNNING WEENIX
--------------
weenix -n

DISPLAY SUMMARY OF EXECUTION
--------------------------------------
./weenix -n | egrep 'Header:|weenix:'


DISPLAY ALL USER MESSAGES WITH HEADERS
--------------------------------------
./weenix -n | egrep 'Kernel2:|weenix:'


DISPLAY TEST "X" MESSAGES  (W/O SYSMSGS)
----------------------------------------
./weenix -n | egrep -w 'TC"X":|weenix:' 

Replace "X" with {8,9,10,11,12,13}  eg: ./weenix -n | egrep -w 'TC9:|weenix:' 

TEST CASES
----------
Please ls in kshell to check the output of the following test cases.
	8) 	Rename Positive Test Scenario: Make Dir. CD into it and make a file 'F1'. Then Rename it 'F2'.
	9) 	Do_Dup Positive Test Scenario: Make Dir. CD into it Open a file 'F1'. Then Duplicate it's FD. Check number of FD's of current proc.
	10) 	Do_Dup2 Positive Test Scenario: proc_kill_all on a list of threads in a KT_RUN.
	11) 	Link Positive Test Scenario: Make Dir. CD into it Open a file 'F1'. Then Link it with new file 'F2'. 
		Both should have increased size count and links.
	12) 	UnLink Positive Test Scenario: Make Dir. CD into it Open a file 'F1'. Then Link and Unlink it with new file 'F2'.
	13) 	RmDirPositive Test Scenario:Make Dir. CD into it Make another Directory Temp. Then Remove It
	14) 	Recursive MkDir Test Scenario: Do MkDir 20 Times and check if it works. -- COMMENTED.

We have tested above test cases along with VFSTEST_MAIN and everything is running SUCCESSFULLY and we're able to HALT CLEANLY.


EXPLANATION OF FUNCTIONS
------------------------
I) FS/vfsyscall.c
	
1. do_read():
	transfers at most count bytes from file into buf. Reads from the file at offset bytes and updates the f_pos. 
	Returns the number of bytes transferred.
	
2. do_write():
	transfers count bytes from buf into file. It begins writing at offset bytes into the file and updates the f_pos.
	Returns the number of bytes transferred.

3. do_close():
	closes the file according to current process's fd using fput.

4. do_dup():
	fetches a new fd from the current process and sets it to the file pointed by the passed fd.

5. do_dup2():
	fetches the old fd and new fd and sets the new fd to the file pointed by the old fd.

6. do_mknod():
	creates a special file for the device specified by 'devid' and an entry for it in 'dir' of the specified name.

7. do_mkdir():
	creates a directory called name in dir.

8. do_rmdir():
	removes the directory called name from dir. 

9. do_unlink():
	removes the link to the vnode in dir specified by name.

10. do_link():
	sets-up a hardlink. links oldvnode into dir with the specified name. This increases the linkcount of the file.

11. do_rename():
	renames the old filename to new filename. this is done by linking the oldfile with the new file name and unlinking the oldfile.

12. do_chdir():
	changes the directory of the current process to the new directory path. Returns errors if the path does not exist.

13. do_getdent():
	reads the directory from current process's fd and fills the dirent structure. Increments the f_pos.

14. do_lseek():
	modifies the f_pos of a fd based on offset and whence (SEEK_SET returns it to 0 and  SEEK_END sets it to the end).

15. do_stat():
	sets the fields in the given buf, filling it with information about file.


II) FS/namev.c

1. lookup():
	Calls the ramfs_lookup to find the vnode corresponding to the name specified in the directory. 
	ramfs_lookup uses vget to increase the refcount for the vnode. returns an error if it cannot find the file.

2. dir_namev():
	this function is used to return the vnode of the parent directory and the name of the file as specified by the path from the base directory. it loops through the pathname fetching the name and len & does a lookup if its' not the basename. It uses vget and vput in every iteration to manage the refcounts.

3. open_namev():
	Uses dir_namev to traverse the filepath and do a lookup on each iteration to the final vnode. It then does a lookup to check if the file in the parent directory fetched using dir_namev exists or not. If the file doesn't exist and O_CREAT file is set, it creates the file.

III) FS/open.c

1. do_open():
	This function is supposed to open the file by using the filename and mode given in the args. This is done by getting an empty file descriptor, calling open_namev with the status_flags (O_CREAT,O_TRUNC,O_APPEND) and setting the vnode of the file descriptor with the node returned by open_namev. File Accesses has been handled by setting the f_mode of the fd.
