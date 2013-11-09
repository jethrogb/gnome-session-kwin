/**
 * This program is free software: you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation, either version 3 of the License, or (at your option) 
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
 * more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * On Debian systems, the complete text of the GNU General
 * Public License version 3 can be found in "/usr/share/common-licenses/GPL-3".
 */

// for execvpe()
#define _GNU_SOURCE

#include <X11/Xlib.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/SM/SMlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

static void set_cursor_theme(const char* pname)
{
}

static int sm_connect_launch_kwin(const char* pname, int argc, char** argv, char** envp)
{
	char* id=NULL;
	char err[256]={0};
	SmcConn conn=SmcOpenConnection(NULL, NULL, 1, 0, 0, NULL, getenv("DESKTOP_AUTOSTART_ID"), &id, 255, err);
	if (conn)
	{
		if (id)
			fprintf(stderr,"%s[%d] New client id: %s\n",pname,getpid(),id);
		SmcCloseConnection(conn, 0, NULL);
		argv[0]="kwin";
		execvpe(argv[0],argv,envp);
		fprintf(stderr,"%s[%d] execvpe error: ",pname,getpid());
		perror(NULL);
		return 0;
	}
	else
	{
		fprintf(stderr,"%s[%d] SmcOpenConnection error: %s\n",pname,getpid(),err);
		return 1;
	}
}

int main(int argc, char** argv, char** envp)
{
	set_cursor_theme(argv[0]);
	
	return sm_connect_launch_kwin(argv[0],argc,argv,envp);
}
