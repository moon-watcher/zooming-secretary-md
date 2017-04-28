
#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/spritedispatcher.h"
#include "../inc/SPRFactory.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define FAN_ATTRIBUTES          ( TILE_ATTR( PAL0, FALSE, FALSE, FALSE ) )
#define TOPIC_ATTRIBUTES 		( TILE_ATTR( PAL1, FALSE, FALSE, FALSE ) )
#define COFFEE_ATTRIBUTES		( TILE_ATTR( PAL1, FALSE, FALSE, FALSE ) )
#define HEART_ATTRIBUTES		( TILE_ATTR( PAL2,  TRUE, FALSE, FALSE ) )
#define SECRETARY_ATTRIBUTES 	( TILE_ATTR( PAL2, FALSE, FALSE, FALSE ) )
#define PHONE_ATTRIBUTES		( TILE_ATTR( PAL2, FALSE, FALSE, FALSE ) )
#define NPC_ATTRIBUTES			( TILE_ATTR( PAL3, FALSE, FALSE, FALSE ) )
#define NPC_ATTRIBUTES_HFLIP	( TILE_ATTR( PAL3, FALSE, FALSE,  TRUE ) )


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createSecretary( s16 positionX, s16 positionY )
{
	Sprite *playerSprite = SPRD_new( 1, SPRD_UP );
	SPR_initSprite( playerSprite, (SpriteDefinition*) &secretarySprDef, positionX, positionY, SECRETARY_ATTRIBUTES );

	return playerSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createGhostNPC( s16 positionX, s16 positionY )
{
	Sprite *ghostNPCSprite = SPRD_new( 1, SPRD_UP );
	SPR_initSprite( ghostNPCSprite, (SpriteDefinition*) &npcGhostSprDef, positionX, positionY, NPC_ATTRIBUTES );

	return ghostNPCSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createChiefNPC( s16 positionX, s16 positionY )
{
	Sprite *chiefNPCSprite = SPRD_new( 2, SPRD_UP );
	SPR_initSprite( chiefNPCSprite, (SpriteDefinition*) &npcChiefSprDef, positionX, positionY, NPC_ATTRIBUTES_HFLIP );

	return chiefNPCSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createBouncerNPC( s16 positionX, s16 positionY )
{
	Sprite *bouncerNPCSprite = SPRD_new( 2, SPRD_UP );
	SPR_initSprite( bouncerNPCSprite, (SpriteDefinition*) &npcBouncerSprDef, positionX, positionY, NPC_ATTRIBUTES );

	return bouncerNPCSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createChatterNPC( s16 positionX, s16 positionY )
{
	Sprite *chatterNPCSprite = SPRD_new( 2, SPRD_UP );
	SPR_initSprite( chatterNPCSprite, (SpriteDefinition*) &npcChatterSprDef, positionX, positionY, NPC_ATTRIBUTES );

	return chatterNPCSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createGeekNPC( s16 positionX, s16 positionY )
{
	Sprite *geekNPCSprite = SPRD_new( 2, SPRD_UP );
	SPR_initSprite( geekNPCSprite, (SpriteDefinition*) &npcGeekSprDef, positionX, positionY, NPC_ATTRIBUTES );

	return geekNPCSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createManboxNPC( s16 positionX, s16 positionY )
{
    Sprite *manboxNPCSprite = SPRD_new( 2, SPRD_UP );
    SPR_initSprite( manboxNPCSprite, (SpriteDefinition*) &npcManboxSprDef, positionX, positionY, NPC_ATTRIBUTES );

    return manboxNPCSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createDibrovNPC( s16 positionX, s16 positionY )
{
    Sprite *dibrovNPCSprite = SPRD_new( 2, SPRD_UP );
    SPR_initSprite( dibrovNPCSprite, (SpriteDefinition*) &npcDibrovSprDef, positionX, positionY, NPC_ATTRIBUTES );

    return dibrovNPCSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createHeart( s16 positionX, s16 positionY )
{
	Sprite *heartSprite = SPRD_new( 3, SPRD_UP );
	SPR_initSprite( heartSprite, (SpriteDefinition*) &heartSprDef, positionX, positionY, HEART_ATTRIBUTES );

	return heartSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createBlaBla( s16 positionX, s16 positionY )
{
	Sprite *blablaSprite = SPRD_new( 3, SPRD_UP );
	SPR_initSprite( blablaSprite, (SpriteDefinition*) &blaSprDef, positionX, positionY, NPC_ATTRIBUTES );

	return blablaSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createCoffee( s16 positionX, s16 positionY )
{
	Sprite *coffeeSprite = SPRD_new( 11, SPRD_UP );
	SPR_initSprite( coffeeSprite, (SpriteDefinition*) &coffeeSprDef, positionX, positionY, COFFEE_ATTRIBUTES );

	return coffeeSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createTopic( s16 positionX, s16 positionY )
{
	Sprite *topicSprite = SPRD_new( 11, SPRD_UP );
	SPR_initSprite( topicSprite, (SpriteDefinition*) &topicSprDef, positionX, positionY, TOPIC_ATTRIBUTES );

	return topicSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createTopicHidden( s16 positionX, s16 positionY )
{
	Sprite *topicSprite = createTopic( positionX, positionY );
	SPR_setFrame( topicSprite, 1 );

	return topicSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createTopicByType( s16 positionX, s16 positionY, u8 topicType )
{
	Sprite *topicSprite = createTopic( positionX, positionY );
	SPR_setAnim( topicSprite, topicType );

	return topicSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createPhone( s16 positionX, s16 positionY )
{
    Sprite *phoneSprite = SPRD_new( 11, SPRD_UP );
    SPR_initSprite( phoneSprite, (SpriteDefinition*) &phoneSprDef, positionX, positionY, PHONE_ATTRIBUTES);

    return phoneSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *createCeilingFan( s16 positionX, s16 positionY )
{
    Sprite *ceilingFanSprite = SPRD_new( 11, SPRD_UP );
    SPR_initSprite( ceilingFanSprite, (SpriteDefinition*) &ceilingFanSprDef, positionX, positionY, FAN_ATTRIBUTES);

    return ceilingFanSprite;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void setHFlip( Sprite *sprite, u8 isHFlip )
{
	if ( isHFlip )
	{
		sprite->attribut |= ( 1 << 11 );
	}
	else
	{
		sprite->attribut &= ~( 1 << 11 );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


const sprFactoryStruct SPRFactory =
{
	.createCoffee		= 	&createCoffee,
	.createHeart		= 	&createHeart,
	.createSecretary	= 	&createSecretary,
	.createTopic		= 	&createTopic,
	.createTopicHidden	= 	&createTopicHidden,
	.createTopicByType	=	&createTopicByType,
	.createPhone		=	&createPhone,
	.createBlaBla		=	&createBlaBla,
	.createChiefNPC		=	&createChiefNPC,
	.createBouncerNPC	=	&createBouncerNPC,
	.createGhostNPC		=	&createGhostNPC,
	.createChatterNPC	=	&createChatterNPC,
	.createGeekNPC		=	&createGeekNPC,
	.createManboxNPC	=	&createManboxNPC,
	.createDibrovNPC    =   &createDibrovNPC,
	.createCeilingFan   =   &createCeilingFan,
	.setHFlip			=	&setHFlip
};


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
