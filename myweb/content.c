#include "headall.h"


void clienterror(int fd, char *cause, char *errnum,
		char *shortmsg, char *longmsg)
{
	char buf[8192], body[8192];

	/* Build the HTTP response body */
	sprintf(body, "<html><title>Web Error</title>");
	sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
	sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
	sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
	sprintf(body, "%s<hr><em>The WBL Web server</em>\r\n", body);

	/* Print the HTTP response */
	sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
	
	write(fd, buf, strlen(buf));
	sprintf(buf, "Content-type: text/html\r\n");
	write(fd, buf, strlen(buf));
	sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
	write(fd, buf, strlen(buf));
	write(fd, body, strlen(body));
	ERRLOG( WAR,"body[%s] strlen[%d]",body, strlen(body));
}
