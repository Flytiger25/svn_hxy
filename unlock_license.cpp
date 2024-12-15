/*******************************************************************/
/*    Copyright (c) 2006-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#include "stdafx.h"
#include "base.hxx"

#ifdef _MSC_VER
#include "license.hxx"
#include "spa_unlock_result.hxx"

// place the spatial_license.h file from the download site in include folder
#include "spatial_license.h"
void unlock_license()
{
	spa_unlock_result out = spa_unlock_products(SPATIAL_LICENSE);
	if (out.get_state() == SPA_UNLOCK_PASS_WARN)
	{ /* insert your warning notification code here */;
	}
	else if (out.get_state() != SPA_UNLOCK_PASS)
	{ /* insert your error handling code here */;
	}
	const char* message = out.get_message_text();
}

#else

// Non licensing platforms
void unlock_license()
{
}

#endif

