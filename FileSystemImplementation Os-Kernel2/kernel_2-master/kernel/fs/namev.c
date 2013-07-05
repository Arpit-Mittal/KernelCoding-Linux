#include "kernel.h"
#include "globals.h"
#include "types.h"
#include "errno.h"

#include "util/string.h"
#include "util/printf.h"
#include "util/debug.h"

#include "fs/dirent.h"
#include "fs/fcntl.h"
#include "fs/stat.h"
#include "fs/vfs.h"
#include "fs/vnode.h"

/* This takes a base 'dir', a 'name', its 'len', and a result vnode.
 * Most of the work should be done by the vnode's implementation
 * specific lookup() function, but you may want to special case
 * "." and/or ".." here depnding on your implementation.
 *
 * If dir has no lookup(), return -ENOTDIR.
 *
 * Note: returns with the vnode refcount on *result incremented.
 */
int
lookup(vnode_t *dir, const char *name, size_t len, vnode_t **result)
{
	/*---KASSERTS----*/
	KASSERT(NULL != dir);
	KASSERT(NULL != name);
	KASSERT(NULL != result);

	/*----Kernel2:VFS:lookup:Begins---*/
	int errno;
	vnode_t *res;

 	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: ------FUNCTION lookup begins\n");
	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: look for name = %s, len = %d in dir vnode %ld\n",name,len,(long)dir->vn_vno);

	/* Check if vnode is not a dir*/
	if (!S_ISDIR(dir->vn_mode))
	{
        	
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: The dir vnode %ld is not a directory\n",(long)dir->vn_vno);
		return -ENOTDIR;
	}

	/* Lookup exists, call it.*/ 
 	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg:  ramfs_lookup called now\n");
	vfs_is_in_use(vfs_root_vn->vn_fs);
	
	if ( len > NAME_LEN )
	{
		
 		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: in lookup : The name is too long\n");
		return -ENAMETOOLONG;
	}

	if ( len == 0 )
	{
		*result = vget(dir->vn_fs,dir->vn_vno);
		return 0;
	}

	errno = dir->vn_ops->lookup(dir,name,len,result);
	vfs_is_in_use(vfs_root_vn->vn_fs);
 	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg: ------FUNCTION lookup ends\n");

	/*---KASSERTS----*/

        return errno;
	/*---Ends----*/
}


/* When successful this function returns data in the following "out"-arguments:
 *  o res_vnode: the vnode of the parent directory of "name"
 *  o name: the `basename' (the element of the pathname)
 *  o namelen: the length of the basename
 *
 * For example: dir_namev("/s5fs/bin/ls", &namelen, &name, NULL,
 * &res_vnode) would put 2 in namelen, "ls" in name, and a pointer to the
 * vnode corresponding to "/s5fs/bin" in res_vnode.
 *
 * The "base" argument defines where we start resolving the path from:
 * A base value of NULL means to use the process's current working directory,
 * curproc->p_cwd.  If pathname[0] == '/', ignore base and start with
 * vfs_root_vn.  dir_namev() should call lookup() to take care of resolving each
 * piece of the pathname.
 *
 * Note: A successful call to this causes vnode refcount on *res_vnode to
 * be incremented.
 */
int
dir_namev(const char *pathname, size_t *namelen, const char **name,
          vnode_t *base, vnode_t **res_vnode)
{
	/*---KASSERTS----*/
	KASSERT(NULL != pathname);
	KASSERT(NULL != res_vnode);

	int idx = 0, errno = 0, lookup_done = 0,len = 0;
 	dbg(DBG_PRINT | DBG_VFS,"Kernel2:SysMsg:  begin dir_namev, pathname = %s\n",pathname);

	/* Set NULL base to crurent pwd */
	if ( base == NULL )
	{
		base = curproc->p_cwd;
	}
	
	/* if pathname starts with /, use root*/
	if (pathname[0] == '/')
	{
		base = vfs_root_vn;
		idx = 1;
	}

	/*Find file (maybe a dir) in current dir*/
	while ( 1 ) 
	{
		/*Loop through pathname, getting name and len*/
		for ( len = 0; pathname[idx+len] != '/'; len++)
		{
			if (pathname[idx+len] == '\0')
			{
				if ( len > NAME_LEN)
				{
					
 					dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg:The name is too long\n");
					return -ENAMETOOLONG;
				}
				KASSERT(NULL != namelen);
				KASSERT(NULL != name);

				/*last vnode found is not a dir*/
				*res_vnode = vget(base->vn_fs,base->vn_vno);
				 KASSERT(NULL != *res_vnode);
				if (!S_ISDIR((*res_vnode)->vn_mode))
				{
					vput(*res_vnode);
					
 					dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: The dir vnode is not a directory\n");
        				return -ENOTDIR;
				}

				*name = &pathname[idx];
				*namelen = len;	
				return 0;
			} 
		}
		/*Do lookup on name IFF its not basename*/
		errno = lookup(base,&pathname[idx],len,res_vnode);
		if (errno < 0)
		{
			return errno;
		} 

		/*Lookup returned ok, so keep going*/
		vput(*res_vnode);
		base = *res_vnode;
		idx = idx + len + 1;
	}

        return 0;
}

/* This returns in res_vnode the vnode requested by the other parameters.
 * It makes use of dir_namev and lookup to find the specified vnode (if it
 * exists).  flag is right out of the parameters to open(2); see
 * <weenix/fnctl.h>.  If the O_CREAT flag is specified, and the file does
 * not exist call create() in the parent directory vnode.
 *
 * Note: Increments vnode refcount on *res_vnode.
 */
int
open_namev(const char *pathname, int flag, vnode_t **res_vnode, vnode_t *base)
{

	/*---KASSERTS----*/
	KASSERT(NULL != pathname);
	KASSERT(NULL != res_vnode);
	/*Use dir_namev to find vnode that MIGHT contain final vnode
	  Then to the last lookup here so that if it fails we can
	  create a new vnode and return that IF the flag is set to 
	  do that. 
	*/
	const char *name;
	size_t len;
	int errno;

	/* call dir_name, pass error back, vput dir */
        errno = dir_namev(pathname,&len,&name,base,res_vnode);
	if ( errno < 0 )
	{
        	return errno;
	}
	vput(*res_vnode);

	/* Lookup name and create if fails */
	errno = lookup(*res_vnode,name,len,res_vnode);
	if ( errno == -ENOENT && (flag & O_CREAT) ==  O_CREAT)
	{
		dbg(DBG_ERROR | DBG_VFS,"Kernel2:SysMsg: open_namev, lookup failed on name = %s, created\n",name);
		KASSERT(NULL != (*res_vnode)->vn_ops->create);
		errno = (*res_vnode)->vn_ops->create(*res_vnode,name,len,res_vnode);	 
	}

	return errno;	
	
}

#ifdef __GETCWD__
/* Finds the name of 'entry' in the directory 'dir'. The name is writen
 * to the given buffer. On success 0 is returned. If 'dir' does not
 * contain 'entry' then -ENOENT is returned. If the given buffer cannot
 * hold the result then it is filled with as many characters as possible
 * and a null terminator, -ERANGE is returned.
 *
 * Files can be uniquely identified within a file system by their
 * inode numbers. */
int
lookup_name(vnode_t *dir, vnode_t *entry, char *buf, size_t size)
{
        NOT_YET_IMPLEMENTED("GETCWD: lookup_name");
        return -ENOENT;
}


/* Used to find the absolute path of the directory 'dir'. Since
 * directories cannot have more than one link there is always
 * a unique solution. The path is writen to the given buffer.
 * On success 0 is returned. On error this function returns a
 * negative error code. See the man page for getcwd(3) for
 * possible errors. Even if an error code is returned the buffer
 * will be filled with a valid string which has some partial
 * information about the wanted path. */
ssize_t
lookup_dirpath(vnode_t *dir, char *buf, size_t osize)
{
        NOT_YET_IMPLEMENTED("GETCWD: lookup_dirpath");

        return -ENOENT;
}
#endif /* __GETCWD__ */
