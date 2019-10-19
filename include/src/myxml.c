#include <stdio.h>
#include <string.h>
#include "myxml.h"


int XmlGetNetwork( char * node, char * des, char * ip,char * port )
{
	xmlDocPtr pdoc;	
	xmlNodePtr pnode, pcur;
	xmlNodePtr nptr;
	int flag;
	char sTmp[50];

	xmlKeepBlanksDefault(0); 
	
	memset( sTmp, 0, sizeof( sTmp ));
	strcpy( sTmp, getenv( "HOME" ) );
	strcat( sTmp, "/etc/Network.xml" );

	pdoc = xmlReadFile( sTmp , "UTF-8", XML_PARSE_RECOVER );
	if (pdoc == NULL)
		return -1;

	pnode = xmlDocGetRootElement (pdoc);
	if (pnode == NULL)
		return -2;
	
	pcur = pnode->xmlChildrenNode;
	
	while( pcur != NULL )
	{
		if( !xmlStrcmp( pcur->name, BAD_CAST( (xmlChar *)node )) )
		{
			nptr = pcur->xmlChildrenNode;
			if( nptr == NULL )
				return -3;
			while( nptr)
			{
				if ( !xmlStrcmp(nptr->name, BAD_CAST((xmlChar *)"Desc")) )
				{
					if( des != NULL )
						strcpy( des, (char *)(XML_GET_CONTENT(nptr->xmlChildrenNode)) );
				}
				else if( !xmlStrcmp(nptr->name, BAD_CAST((xmlChar *)"IP")) )
				{
					if( ip != NULL )
						strcpy( ip, (char *)(XML_GET_CONTENT(nptr->xmlChildrenNode)) );
				}
				else if( !xmlStrcmp(nptr->name, BAD_CAST((xmlChar *)"Port")) )
				{
					if( port != NULL )
						strcpy( port, (char *)(XML_GET_CONTENT(nptr->xmlChildrenNode)) );
				}
				flag++;		// 子节点数量
				if( flag == 3 )
					break;
				nptr = nptr->next;
			}
		}

		pcur = pcur->next;
	}

	xmlFreeDoc (pdoc);
	xmlCleanupParser ();
	xmlMemoryDump ();
	return 1;
}

