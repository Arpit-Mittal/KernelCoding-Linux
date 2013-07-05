/*
 *  FILE: vfs_syscall.c
 *  AUTH: mcc | jal
 *  DESC:
 *  DATE: Wed Apr  8 02:46:19 1998
 *  $Id: vfs_syscall.c,v 1.1 2012/10/10 20:06:46 william Exp $
 */

#include "kernel.h"
#include "errno.h"
#include "globals.h"
#include "fs/vfs.h"
#include "fs/file.h"
#include "fs/vnode.h"
#include "fs/vfs_syscall.h"
#include "fs/open.h"
#include "fs/fcntl.h"
#include "fs/lseek.h"
#include "mm/kmalloc.h"
#include "util/string.h"
#include "util/printf.h"
#include "fs/stat.h"
#include "util/debug.h"

/* To read a file:
 *      o fget(fd)
 *      o call its virtual read f_op
 *      o update f_pos
 *      o fput() it
 *      o return the number of bytes read, or an error
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        fd is not a valid file descriptor or is not open for reading.
 *      o EISDIR
 *        fd refers to a directory.
 *
 * In all cases, be sure you do not leak file refcounts by returning before
 * you fput() a file that you fget()'ed.
 */
int
do_read(int fd, void *buf, size_t nbytes)
{
       /*----------------Kernel2-----------------------*/
	dbg(DBG_PRINT,"Kernel2:SysMsg: do_read begins and my fd = %d\n",fd);
	file_t *f;
	int n_bytes;

	if(fd < 0 || fd >= NFILES) 
	{
		dbg(DBG_ERROR,"Kernel2:SysMsg:do_read returned Bad file descriptor");	
		return -EBADF;
	}

	f = fget(fd);

	if(f == NULL)
	{	
		dbg(DBG_ERROR,"Kernel2:SysMsg:do_read returned Bad file descriptor");	
		return -EBADF;
	}

	if( (f->f_mode & FMODE_READ) != FMODE_READ )
	{	
		dbg(DBG_ERROR,"Kernel2:SysMsg:do_read returned Bad file descriptor");	
		fput(f);
		return -EBADF;
	}

	/*fd is a directory*/
	if(S_ISDIR(f->f_vnode->vn_mode))
	{	
		dbg(DBG_PRINT,"Kernel2:SysMsg:do_read returned Bad file descriptor");	
		fput(f);
		return -EBADF;
	}

	KASSERT(NULL != f->f_vnode->vn_ops->read);	
	n_bytes = f->f_vnode->vn_ops->read(f->f_vnode,f->f_pos,buf,nbytes);
	f->f_pos += n_bytes;
		
	fput(f);
	dbg(DBG_SYSCALL,"Kernel2:SysMsg: do_read reads : %d bytes\n",n_bytes);
	return n_bytes;
	/*----------------------------------------------*/
	
	
}

/* Very similar to do_read.  Check f_mode to be sure the file is writable.  If
 * f_mode & FMODE_APPEND, do_lseek() to the end of the file, call the write
 * f_op, and fput the file.  As always, be mindful of refcount leaks.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        fd is not a valid file descriptor or is not open for writing.
 */
int
do_write(int fd, const void *buf, size_t nbytes)
{
	
	/*----------------Kernel2-----------------------*/
	dbg(DBG_PRINT,"Kernel2:SysMsg: do_write begins and my fd = %d\n",fd);
	int n_bytes;
	file_t *f = NULL;

	
	if(fd < 0 || fd >= NFILES) 
	{
		dbg(DBG_ERROR,"Kernel2:SysMsg: do_write returns due to bad fd\n");
		return -EBADF;
	}

	f = fget(fd);

	if(f == NULL)
	{	
		dbg(DBG_ERROR,"Kernel2:SysMsg: do_write returns due to bad fd\n");
		return -EBADF;
	}

	if( (f->f_mode & FMODE_WRITE) != FMODE_WRITE )
	{	
		fput(f);
		dbg(DBG_ERROR,"Kernel2:SysMsg: do_write returns due to bad fd\n");
		return -EBADF;
	}

	/* if append, move to end*/
	if( (f->f_mode & FMODE_APPEND) == FMODE_APPEND)
	{
		f->f_pos = do_lseek(fd,0,SEEK_END);	
	}	

	KASSERT(NULL != f->f_vnode->vn_ops->read);	
	n_bytes = f->f_vnode->vn_ops->write(f->f_vnode,f->f_pos,buf,nbytes);
	f->f_pos += n_bytes;
	 KASSERT((S_ISCHR(f->f_vnode->vn_mode)) || (S_ISBLK(f->f_vnode->vn_mode)) ||((S_ISREG(f->f_vnode->vn_mode)) && (f->f_pos <= f->f_vnode->vn_len)));	

	fput(f);
	dbg(DBG_PRINT,"Kernel2:SysMsg: do_write writes: %d\n",n_bytes);
	return n_bytes;
	
	/*----------------------------------------------*/
	
	
}

/*
 * Zero curproc->p_files[fd], and fput() the file. Return 0 on success
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        fd isn't a valid open file descriptor.
 */
int
do_close(int fd)
{	
	/*---------------------Kernel2------------------------------------*/
	file_t *f;
	dbg(DBG_PRINT,"Kernel2:SysMsg: do_close attempts to close file with fd equals: %d \n",fd);

	if(fd < 0 || fd >= NFILES) 
	{
		dbg(DBG_ERROR,"Kernel2:SysMsg: do_close returns due to bad fd\n");
		return -EBADF;
	}

	f = fget(fd);	

	if(f == NULL) 
	{
		dbg(DBG_ERROR,"Kernel2:SysMsg: do_close returns due to bad fd\n");
		return -EBADF;	
	}

	fput(f);
	fput(f);
	curproc->p_files[fd] = NULL;

	dbg(DBG_PRINT,"Kernel2:SysMsg: do_close successful\n");
        return 0;
	/*---------------------------------------------------------------*/
}

/* To dup a file:
 *      o fget(fd) to up fd's refcount
 *      o get_empty_fd()
 *      o point the new fd to the same file_t* as the given fd
 *      o return the new file descriptor
 *
 * Don't fput() the fd unless something goes wrong.  Since we are creating
 * another reference to the file_t*, we want to up the refcount.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        fd isn't an open file descriptor.
 *      o EMFILE
 *        The process already has the maximum number of file descriptors open
 *        and tried to open a new one.
 */
int
do_dup(int fd)
{
	/*----------------------------Kernel2---------------------------*/
	file_t *f = NULL;
	int nfd = 0;

	if(fd < 0 || fd >= NFILES )
	{
		dbg(DBG_ERROR,"Kernel2:SysMsg: do_dup returns due to bad fd\n");
		return -EBADF;	
	} 

	f = fget(fd);

	if ( f == NULL )
	{
		dbg(DBG_ERROR,"Kernel2:SysMsg: do_dup returns due to bad fd\n");
		return -EBADF;
	}

	nfd = get_empty_fd(curproc);

	if(nfd < 0 )
	{
		fput(f);
		dbg(DBG_ERROR,"Kernel2:SysMsg: do_dup returns due to bad fd\n");
		return nfd;	
	}       

	curproc->p_files[nfd] = f;
        return nfd;
	/*---------------------------------------------------------------*/
}

/* Same as do_dup, but insted of using get_empty_fd() to get the new fd,
 * they give it to us in 'nfd'.  If nfd is in use (and not the same as ofd)
 * do_close() it first.  Then return the new file descriptor.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        ofd isn't an open file descriptor, or nfd is out of the allowed
 *        range for file descriptors.
 */
int
do_dup2(int ofd, int nfd)
{
	/*---------------------------------Kernel2---------------------------*/
	file_t *f = NULL;
	int errno = 0;
	if( nfd < 0 || nfd >= NFILES || ofd < 0 || ofd >= NFILES ) 
	{
		dbg(DBG_ERROR,"Kernel2:SysMsg: do_dup2 returns due to bad fd\n");
		return -EBADF;
	}

	
	f = fget(ofd);

	if ( f == NULL ) 
	{
		dbg(DBG_ERROR,"Kernel2:SysMsg: do_dup2 returns due to bad fd\n");
		return -EBADF;
	}
	
	if(ofd  == nfd)
	{
		fput(f);				
		return nfd;
	}	

	if(curproc->p_files[nfd] && nfd != ofd) 
	{
		errno = do_close(nfd);	
		if (errno < 0)
		{
			fput(f);
			dbg(DBG_ERROR,"Kernel2:SysMsg: do_close returns due to bad fd\n");
			return errno;
		}
	} 

	curproc->p_files[nfd] = f;
        return nfd;
	/*------------------------------------------------------------------*/
}

/*
 * This routine creates a special file of the type specified by 'mode' at
 * the location specified by 'path'. 'mode' should be one of S_IFCHR or
 * S_IFBLK (you might note that mknod(2) normally allows one to create
 * regular files as well-- for simplicity this is not the case in Weenix).
 * 'devid', as you might expect, is the device identifier of the device
 * that the new special file should represent.
 *
 * You might use a combination of dir_namev, lookup, and the fs-specific
 * mknod (that is, the containing directory's 'mknod' vnode operation).
 * Return the result of the fs-specific mknod, or an error.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EINVAL
 *        mode requested creation of something other than a device special
 *        file.
 *      o EEXIST
 *        path already exists.
 *      o ENOENT
 *        A directory component in path does not exist.
 *      o ENOTDIR
 *        A component used as a directory in path is not, in fact, a directory.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 */
int
do_mknod(const char *path, int mode, unsigned devid)
{
        /*--------------------Kernel2---------------------*/
	
	const char *name;
	size_t len;
	int errno;
	vnode_t *res_vnode; 

	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: mknode enters with path = %s\n",path);
	if(mode != S_IFCHR || mode != S_IFCHR || strlen(path) < 1)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_mknod returns due to invalid argument\n");
		return -EINVAL;
	}
	
	if(strlen(path) > MAXPATHLEN)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_mknod returns due to Path name too long\n");
		return -ENAMETOOLONG;
	}	

        errno = dir_namev(path,&len,&name,NULL,&res_vnode);
	if ( errno < 0 )
	{
        	return errno;
	}
	vput(res_vnode);

	/* Lookup name and create if fails */
	errno = lookup(res_vnode,name,len,&res_vnode);
	switch ( errno )
	{
		case 0  :
			dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_mknod returns, file exists\n");
			vput(res_vnode);
			return -EEXIST;
		case -ENOENT :
			KASSERT(NULL != res_vnode->vn_ops->mknod);	
			errno = res_vnode->vn_ops->mknod(res_vnode,name,len,mode,devid);
			return errno;
		default :
			return errno;
	} 

        return -1;
	
	/*----------------------------------------------------*/
}

/* Use dir_namev() to find the vnode of the dir we want to make the new
 * directory in.  Then use lookup() to make sure it doesn't already exist.
 * Finally call the dir's mkdir vn_ops. Return what it returns.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EEXIST
 *        path already exists.
 *      o ENOENT
 *        A directory component in path does not exist.
 *      o ENOTDIR
 *        A component used as a directory in path is not, in fact, a directory.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 */
int
do_mkdir(const char *path)
{
        /*------------------Kernel2------------------------------*/
	const char *name;
	size_t len;
	int errno;
	vnode_t *res_vnode; 
	if(strlen(path) < 1)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_mkdir returns due to invalid argument\n");
		return -EINVAL;
	}

	if( strlen(path) > MAXPATHLEN )
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_mkdir returns, component of path too long\n");
		return -ENAMETOOLONG;
	}
	
        errno = dir_namev(path,&len,&name,NULL,&res_vnode);
	if ( errno < 0 )
	{
        	return errno;
	}
	vput(res_vnode);
	
	/* Lookup name and create if fails */
	errno = lookup(res_vnode,name,len,&res_vnode);
	switch ( errno )
	{
		case 0  :
			vput(res_vnode);
			dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_mkdir returns, directory already exist\n");
			return -EEXIST;
		case -ENOENT :
			KASSERT(NULL != res_vnode->vn_ops->mkdir);
			errno = res_vnode->vn_ops->mkdir(res_vnode,name,len);
        		return errno;
		default :
			return errno;
	} 

        return -1;
	/*-----------------------------------------------------------*/
}

/* Use dir_namev() to find the vnode of the directory containing the dir to be
 * removed. Then call the containing dir's rmdir v_op.  The rmdir v_op will
 * return an error if the dir to be removed does not exist or is not empty, so
 * you don't need to worry about that here. Return the value of the v_op,
 * or an error.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EINVAL
 *        path has "." as its final component.
 *      o ENOTEMPTY
 *        path has ".." as its final component.
 *      o ENOENT
 *        A directory component in path does not exist.
 *      o ENOTDIR
 *        A component used as a directory in path is not, in fact, a directory.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 */
int
do_rmdir(const char *path)
{
	/*---------------------Kernel2------------------------*/
	const char *name;
	size_t len;
	int errno;
	vnode_t *res_vnode; 

	dbg(DBG_PRINT | DBG_VFS, "Kernel2:SysMsg: rmdir begins looking for path = %s\n",path);

	if(strlen(path) < 1)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_rmdir returns due to invalid argument\n");
		return -EINVAL;
	}


	if(strlen(path) > MAXPATHLEN)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_rmdir returns due to the component of path too long\n");
		return -ENAMETOOLONG;
	}        
	
	/*find the parrent dir*/
	errno = dir_namev(path,&len,&name,NULL,&res_vnode);
	if ( errno < 0 )
	{
        	return errno;
	}

	/*checks for the . and .. as the final component*/ 
	if(name_match(".",name,len))
	{
		vput(res_vnode);
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_rmdir returns due to . as the final path component\n");
		return -EINVAL;	
	}
	if(name_match("..", name,len))
	{
		vput(res_vnode);
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_rmdir returns due to .. as the finalpath component\n");
		return -ENOTEMPTY;
	}

	/*remove the dir*/
	KASSERT(NULL != res_vnode->vn_ops->rmdir);
	errno = res_vnode->vn_ops->rmdir(res_vnode,name,len);
	vput(res_vnode);
	if (errno < 0)
	{
		return errno;
	}
	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: do_rmdir ends sussessfully\n");
	return 0;
	
        /*---------------------------------------------------*/
}

/*
 * Same as do_rmdir, but for files.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EISDIR
 *        path refers to a directory.
 *      o ENOENT
 *        A component in path does not exist.
 *      o ENOTDIR
 *        A component used as a directory in path is not, in fact, a directory.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 */
int
do_unlink(const char *path)
{
	/*----------------------Ishita--------------------------*/
	const char *name;
	size_t len;
	int errno;
	vnode_t *res_vnode, *dir_vnode; 

	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: Chris:unlink begins looking for path  = %s\n",path);

	if( strlen(path) < 1 )
	{
		return -EINVAL;
	}

	if(strlen(path) > MAXPATHLEN)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_unlink returns, path name too long\n");
		return -ENAMETOOLONG;
	}        

	/*To resolve the path and reach to the file that needs to be unlinked*/
        errno = dir_namev(path,&len,&name,NULL,&dir_vnode);
	if ( errno < 0 )
	{	
        	return errno;
	}

	/* Lookup name and create if fails */
	errno = lookup(dir_vnode,name,len,&res_vnode);
	if ( errno < 0 )
	{
		vput(dir_vnode);
		return errno;
	}

	if (S_ISDIR(res_vnode->vn_mode))
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_unlink returns, res_vnode is a directory\n");
		vput(dir_vnode);
		vput(res_vnode);
		return -EISDIR;
	}


	/*unlink and vput*/
	
	KASSERT(NULL != dir_vnode->vn_ops->unlink);
	errno = dir_vnode->vn_ops->unlink(dir_vnode,name,len);
	vput(dir_vnode);
	vput(res_vnode);
	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: do_unlink end successful\n");
       	return errno;

	/*-------------------------------------------------------*/
	
}

/* To link:
 *      o open_namev(from)
 *      o dir_namev(to)
 *      o call the destination dir's (to) link vn_ops.
 *      o return the result of link, or an error
 *
 * Remember to vput the vnodes returned from open_namev and dir_namev.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EEXIST
 *        to already exists.
 *      o ENOENT
 *        A directory component in from or to does not exist.
 *      o ENOTDIR
 *        A component used as a directory in from or to is not, in fact, a
 *        directory.
 *      o ENAMETOOLONG
 *        A component of from or to was too long.
 */
int
do_link(const char *from, const char *to)
{
	/*---------------Kernel2------------*/
	const char *name;	
	vnode_t *res_from_vnode;
	vnode_t *res_to_vnode;
	size_t len;
	int errno;

	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg:Do_Link: begins: parameters : from = %s : to = %s\n",from, to);
	
	/*----ENAMETOOLONG handled----*/

	if((strlen(to) < 1 ) || strlen(from) < 1)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_link returns due to invalid argument\n");
		return -EINVAL;
	}

	if(strlen(from) > MAXPATHLEN || strlen(to) > MAXPATHLEN)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_link returns, name too long\n");
		return -ENAMETOOLONG;
	}
	
	/*fetch the vnode associated with 'from' filepath*/
	errno = open_namev(from,0,&res_from_vnode,NULL);
	if (errno < 0)
	{
		return errno;
	}

	/*find dir vnode holder name to*/
	errno = dir_namev(to,&len,&name,NULL,&res_to_vnode);
	if ( errno < 0 )
	{
		vput(res_from_vnode);
        	return errno;
	}
	vput(res_to_vnode);
	
	errno = lookup(res_to_vnode,name,len,&res_to_vnode);
	switch ( errno )
	{
		case 0  :
			dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_link returns, because the name already exist\n");
			vput(res_to_vnode);
			vput(res_from_vnode);
			return -EEXIST;
		case -ENOENT :
			errno = res_to_vnode->vn_ops->link(res_from_vnode, res_to_vnode, name, len);
			vput(res_from_vnode);
        		return errno;
		default :
			vput(res_from_vnode);
			return errno;
	} 

	return -1;
	/*------------------------------*/
}

/*      o link newname to oldname
 *      o unlink oldname
 *      o return the value of unlink, or an error
 *
 * Note that this does not provide the same behavior as the
 * Linux system call (if unlink fails then two links to the
 * file could exist).
 */
int
do_rename(const char *oldname, const char *newname)
{
	/*---------------Kernel2------------*/
	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: do_rename begins\n");

	if(strlen(oldname) < 1 || strlen(newname) < 1)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_rename returns due to invalid argument\n");
		return -EINVAL;
	}


	do_link(oldname, newname);
	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: do_rename ends\n");

	return do_unlink(oldname);
	/*----------------------------------*/
}

/* Make the named directory the current process's cwd (current working
 * directory).  Don't forget to down the refcount to the old cwd (vput()) and
 * up the refcount to the new cwd (open_namev() or vget()). Return 0 on
 * success.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o ENOENT
 *        path does not exist.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 *      o ENOTDIR
 *        A component of path is not a directory.
 */
int
do_chdir(const char *path)
{
	/*---------------Kernel2------------*/
	int errno;
	vnode_t *res_vnode;
	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: do_chdir begins\n");
	
	if(strlen(path) < 1)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_chdir returns due to invalid argument\n");
		return -EINVAL;
	}

	if(strlen(path) > MAXPATHLEN)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_chdir returns, name too long\n");
		return -ENAMETOOLONG;
	}

	/*open/increase the refcount of the vnode specified by the path*/
	errno = open_namev(path,0,&res_vnode,NULL);
	if(errno < 0)
	{
		return errno;
	}

	/* error if cd to file*/
	if(!S_ISDIR(res_vnode->vn_mode))
	{	
		vput(res_vnode);
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_chdir returns,entry not a directory\n");
		return -ENOTDIR;
	}
			
	/* decrement the refcount of old cwd */
	vput(curproc->p_cwd);

	/* change the parent directory of the current process */
	curproc->p_cwd = res_vnode;
	dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_chdir ends\n");
	return 0;

	/*----------------------------------*/
}

/* Call the readdir f_op on the given fd, filling in the given dirent_t*.
 * If the readdir f_op is successful, it will return a positive value which
 * is the number of bytes copied to the dirent_t.  You need to increment the
 * file_t's f_pos by this amount.  As always, be aware of refcounts, check
 * the return value of the fget and the virtual function, and be sure the
 * virtual function exists (is not null) before calling it.
 *
 * Return either 0 or sizeof(dirent_t), or -errno.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        Invalid file descriptor fd.
 *      o ENOTDIR
 *        File descriptor does not refer to a directory.
 */
int
do_getdent(int fd, struct dirent *dirp)
{
	/*------------------Kernel2------------------*/
	int n_bytes;
      	file_t *f;

	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: do_getdent begins\n");
		
	if(fd < 0 || fd >= NFILES)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_getdent returns due to bad fd\n");
		return -EBADF;	
	} 

	f=fget(fd);

	if ( f == NULL ) 
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_getdent returns due to bad fd\n");
		return -EBADF;
	}

	/*check its not a dir*/
	if(!S_ISDIR(f->f_vnode->vn_mode))
	{	
		fput(f);
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_getdent returns, entry not a directory\n");
		return -ENOTDIR;
	}
			
	n_bytes=f->f_vnode->vn_ops->readdir(f->f_vnode,f->f_pos,dirp);
	f->f_pos += n_bytes;
	fput(f);

	if ( n_bytes == 0 )
	{
		return 0;
	}
	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: do_getdent ends\n");
        return sizeof(*dirp);
	/*------------------------------------------------*/
}

/*
 * Modify f_pos according to offset and whence.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        fd is not an open file descriptor.
 *      o EINVAL
 *        whence is not one of SEEK_SET, SEEK_CUR, SEEK_END; or the resulting
 *        file offset would be negative.
 */
int
do_lseek(int fd, int offset, int whence)
{
       
	/*-----------------Kernel2---------------------------*/
	file_t *f;	

	/*whence means the origin of the file*/
	if( whence!= SEEK_SET && whence!= SEEK_CUR && whence!= SEEK_END )	
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_lseek returns due to invalid argument\n");
		return -EINVAL;	
	}

	if ( fd < 0 || fd >= NFILES )
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_lseek returns due to bad fd\n");
		return -EBADF;
	}

	f = fget(fd);

	if( f == NULL )
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_lseek returns due to bad fd\n");
		return -EBADF;	
	} 
	
	
	switch ( whence )
	{
		case SEEK_SET : 
			f->f_pos = offset;
			break;
		case SEEK_END :
			f->f_pos = f->f_vnode->vn_len + offset;
			break;
		case SEEK_CUR :
			f->f_pos += offset;
	}


	/* Check for negative offset and throw error*/
	if ( f->f_pos < 0 )
	{
		f->f_pos = 0;
		fput(f);
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_lseek returns due to invalid argument\n");
		return -EINVAL;
	}

	fput(f);
	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: do_mkdir ends\n");
	return f->f_pos;
	/*---------------------------------------------------*/
}

/*
 * Find the vnode associated with the path, and call the stat() vnode operation.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o ENOENT
 *        A component of path does not exist.
 *      o ENOTDIR
 *        A component of the path prefix of path is not a directory.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 */
int
do_stat(const char *path, struct stat *buf)
{

	/*-------------Kernel2-----------------*/
	const char *name;
        size_t len;
        int errno = 0, mystat = 0;;
	vnode_t *res_vnode;
	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: do_stat begins\n");

	if( strlen(path) > MAXPATHLEN)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_stat returns, path name too long\n");
		return -ENAMETOOLONG;
	}
	
	if(strlen(path)<1)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: do_stat returns due to invalid argument\n");
		return -EINVAL;
	}
	errno = dir_namev(path,&len,&name,NULL,&res_vnode);
        if ( errno < 0 )
        {
                return errno;
        }
	vput(res_vnode);

        /* Lookup name and create if fails */
	errno = lookup(res_vnode,name,len,&res_vnode);
	if ( errno < 0 )
        {
		return errno;
        }

	/*do stat THEN vput the vnode back down*/
	
	KASSERT(NULL != res_vnode->vn_ops->stat);
	errno = res_vnode->vn_ops->stat(res_vnode,buf);
	vput(res_vnode);
	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: do_stat ends\n");
        return errno;
	/*---------------------------------------*/
}

#ifdef __MOUNTING__
/*
 * Implementing this function is not required and strongly discouraged unless
 * you are absolutely sure your Weenix is perfect.
 *
 * This is the syscall entry point into vfs for mounting. You will need to
 * create the fs_t struct and populate its fs_dev and fs_type fields before
 * calling vfs's mountfunc(). mountfunc() will use the fields you populated
 * in order to determine which underlying filesystem's mount function should
 * be run, then it will finish setting up the fs_t struct. At this point you
 * have a fully functioning file system, however it is not mounted on the
 * virtual file system, you will need to call vfs_mount to do this.
 *
 * There are lots of things which can go wrong here. Make sure you have good
 * error handling. Remember the fs_dev and fs_type buffers have limited size
 * so you should not write arbitrary length strings to them.
 */
int
do_mount(const char *source, const char *target, const char *type)
{
        NOT_YET_IMPLEMENTED("MOUNTING: do_mount");
        return -EINVAL;
}

/*
 * Implementing this function is not required and strongly discouraged unless
 * you are absolutley sure your Weenix is perfect.
 *
 * This function delegates all of the real work to vfs_umount. You should not worry
 * about freeing the fs_t struct here, that is done in vfs_umount. All this function
 * does is figure out which file system to pass to vfs_umount and do good error
 * checking.
 */
int
do_umount(const char *target)
{
        NOT_YET_IMPLEMENTED("MOUNTING: do_umount");
        return -EINVAL;
}
#endif
