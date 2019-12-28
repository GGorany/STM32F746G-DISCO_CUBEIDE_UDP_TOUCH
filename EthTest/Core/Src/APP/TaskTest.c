/*
 * TaskComm.cpp
 *
 *  Created on: Dec 15, 2019
 *      Author: DHKim
 */


/**********************************************************************************************************************
* MODULES USED
**********************************************************************************************************************/
#include "TaskTest.h"
#include "../Lib/emlist.h"
#include "ethernetif.h"
#include "lwip.h"
#include "lwip/api.h"
#include "ip_addr.h"
#include <string.h>

/**********************************************************************************************************************
* DEFINITIONS AND MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
* TYPEDEFS AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
* PROTOTYPES OF FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
* DECLARATIONS OF VARIABLES
**********************************************************************************************************************/
LinkedList* listTouch = NULL;

static struct netconn *conn = NULL;
static struct netbuf *buf;

const ip_addr_t des_addr = IPADDR4_INIT(0x0600a8c0); //    6.0.168.192


/*--------------------------------------------------------------------------*/
/* Name      | Task_Test_Init                                               */
/*--------------------------------------------------------------------------*/
void Task_Test_Init(void)
{
	listTouch = emlist_create();
}


/*--------------------------------------------------------------------------*/
/* Name      | Task_Test                                                    */
/*--------------------------------------------------------------------------*/
void Task_Test(void)
{
	touchXY* tXY = NULL;

	if (0 == emlist_size(listTouch))
	{
		return;
	}

	tXY = (touchXY*)emlist_pop(listTouch);

	// UDP
	if (conn == NULL)
	{
		conn = netconn_new(NETCONN_UDP);
		(void)netconn_bind(conn, IP_ADDR_ANY, 8089);
		(void)netconn_connect(conn, &des_addr, 8089);
	}

	if (conn != NULL)
	{
		buf = netbuf_new();
		netbuf_alloc(buf, 8);
		buf->p->payload = (char*)tXY->bytes;
		buf->addr.addr = 0;

		(void)netconn_send(conn, buf);

		netbuf_delete(buf);
	}

	LIST_FREE(tXY);
}

/*--------------------------------------------------------------------------*/
/* Name      | Touch_CallBack                                               */
/*--------------------------------------------------------------------------*/
void Touch_CallBack(int32_t x, int32_t y)
{
	touchXY* tXY = (touchXY*)LIST_MALLOC(sizeof(touchXY));
	tXY->position.x = x;
	tXY->position.y = y;

	(void)emlist_insert(listTouch, (void*)tXY);
}

