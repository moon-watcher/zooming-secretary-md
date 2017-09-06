#include <genesis.h>
#include "../inc/message.h"
#include "../inc/topic.h" //TODO: USADO POR TOPICMAX


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define MESSAGE_PALETTE ( PAL1 )
#define MESSAGE_ATTRIBUTES(idx) ( TILE_ATTR_FULL( MESSAGE_PALETTE, FALSE, FALSE, FALSE, idx ) )
#define MESSAGE_PLAN ( PLAN_A )

#define MESSAGE_MAXIMUN_CHARACTERS      30

#define TOTAL_OF_GEOGRAPHY_MESSAGES		30
#define TOTAL_OF_HISTORY_MESSAGES 		21
#define TOTAL_OF_LITERATURE_MESSAGES	30
#define TOTAL_OF_MATH_MESSAGES	        30

#define MESSAGE_BONUS_SUCCESS	"   WELL DONE, YOU GET BONUS!  "
#define MESSAGE_BONUS_FAIL		"       SORRY, NO BONUS!       "


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


typedef struct
{
    const u8 totalOfMessages;
	const char *messages[ MESSAGE_MAXIMUN_CHARACTERS ];

} messagesByTopicStruct;


static const messagesByTopicStruct allAvailableMessages[ TOPIC_MAX ] =
{
	{
		TOTAL_OF_GEOGRAPHY_MESSAGES,
		{
			" MARCO POLO TRAVELLED TO ASIA ",   //00
			"MAGELLAN CROSSED PACIFIC OCEAN",   //01
			"  MT. EVEREST IS 8850 M HIGH  ",   //02
			" AMUNDSEN REACHED SOUTH POLE  ",   //03
			"DR LIVINGSTONE EXPLORED AFRICA",   //04
			"VASCO DA GAMA SAILED TO INDIA ",   //05
			"EARTH RADIUS IS ABOUT 6370 KM ",   //06
			" MARIANA TRENCH IS 11 KM DEEP ",   //07
			"  WATER COVERS 71% OF EARTH   ",   //08
            " MT. FUJI IS LOCATED IN JAPAN ",	//09
			"STONE STATUES IN EASTER ISLAND",	//10
			"JAPAN IS A BIG ISLAND IN ASIA ",	//11
			"MOSCOW, THE CAPITAL OF RUSSIA ",	//12
			"  NILE RIVER IS 6853 KM LONG  ",	//13
			"RUSSIA IS THE LARGEST COUNTRY ",	//14
			"672 COUNTRY CODE FOR ANTARTICA",	//15
			"CASPIAN SEA IS IN FACT A LAKE ",	//16
			"   THERE ARE SIX CONTINENTS   ",	//17
			"    THERE ARE FIVE OCEANS     ",	//18
			"SEAS AND OCEANS ARE SALTWATER ",	//19
			" AMAZON RIVER IS THE BIGGEST  ",	//19
			" PACIFIC OCEAN IS THE LARGEST ",	//20
			"STONEHENGE IS BASED IN ENGLAND",	//21
			"  ATLANTIS IS A LOST ISLAND   ",	//22
			"SPAIN THE MOST LOVELY COUNTRY ",	//23
			" TWO SPANISH CITIES IN AFRICA ",	//24
			"SPAIN BEST OLIVE OIL PRODUCER "	//25
			"MT. TEIDE IS A SPANISH VOLCANO",	//26
			"OVER 1 BILLION PEOPLE IN INDIA",	//27
			" 376 COUNTRY CODE FOR ANDORRA ",	//28
			"BANGKOK IS THE CAPITAL OF THAI"	//29
		}
	},

	{
		TOTAL_OF_HISTORY_MESSAGES,
		{
			"MACBETH WAS A KING OF SCOTLAND",   //00
			"3 KINGDOMS - OF EGYPT'S GLORY ",   //01
			"JERICHO - EARLIEST KNOWN CITY ",   //02
			" LOUIS XIV RULED FOR 72 YEARS ",   //03
			"ANNO DOMINI = YEAR OF OUR LORD",   //04
			"   CE MEANS COMMON ERA = AD   ",   //05
			"STONEHENGE IS OLDER THAN 4200 ",   //06
			" VICTORIA RULED 1837 TO 1901  ",   //07
			"SNOW IN SAHARA ON 18 FEB 1979 ",   //08
			"IN 1492 AMERICA WAS DISCOVERED",   //09
            "SPANISH CIVIL WAR BEGAN - 1936",   //10
            "THE BERLIN WALL FALLS IN 1989 ",   //11
            "   AZTECS WERE POLYTHEISTIC   ",   //12
            "FLEMING DISCOVERED PENICILLIN ",   //13
            "ALFRED NOBEL INVENTED DYNAMITE",   //14
            "SINKING OF THE TITANIC IN 1912",   //15
            " MARCO POLO ARRIVED IN CHINA  ",   //16
            "   CHE GUEVARA DIED IN 1967   ",   //17
            "MEN WALKED ON THE MOON - 1969 ",   //18
            "   GANDHI WAS BORN IN INDIA   ",   //19
            "MANDELA WAS 27 YEARS IN PRISON",   //20
            " RUTH HANDLER INVENTED BARBIE ",   //21
            "LINUS TORVALDS DEVELOPED LINUX",   //22
            "   AL CAPONE WAS A GANGSTER   ",   //23
            "THE VIETNAM WAR ENDED IN 1975 ",   //24
            "  THE GULF WAR ENDED IN 1991  ",   //25
            "  FIDEL CASTRO DIED IN 2008   ",   //26
            "  ELVIS PRESLEY DIED IN 1977  ",   //27
            " FIRST BROADCAST TV IN PARIS  ",   //28
            " TUPAC DIED IN 1996 (AGED 25) "    //29
		}
	},

	{
		TOTAL_OF_LITERATURE_MESSAGES,
		{
			"RHYMES ARE EASIER TO REMEMBER ",   //00
			"VIRGIL IS THE AUTHOR OF AENEID",   //01
			"   OVID WROTE METAMORPHOSES   ",   //02
			"EURIPIDES WON PRIZE FOR MEDEA ",   //03
			"AESCHYLUS INTRODUCED DIALOGUE ",   //04
			"   TAO TE CHING IS BY LAOZI   ",   //05
			"DIVINE COMEDY WRITTEN BY DANTE",   //06
			" PUSHKIN WROTE EUGENE ONEGIN  ",   //07
			" ONEGIN IS ABOUT TRENDY DANDY ",   //08
			"  COLERIDGE WROTE KUBLA KHAN  ",   //09
			" CERVANTES WROTE DON QUIXOTE  ",   //10
			" MACHADO  WAS A SPANISH POET  ",   //11
			" PAPER WAS INVENTED IN CHINA  ",   //12
			"  PAPYRUS WAS USED IN EGYPT   ",   //13
			"   TOLKIEN WROTE THE HOBBIT   ",   //14
            " DOES THE NECRONOMICON EXIST? ",   //15
            "MARIO PUZO WROTE THE GODFATHER",   //16
            "JAWS WRITTEN BY PETER BENCHLEY",   //17
            " GOOSEBUMPS WRITTEN BY STINE  ",   //18
            " MANGA IS THE JAPANESE COMIC  ",   //19
            " EL TEBEO IS THE SPANISH COMIC",   //20
            " FIVE VOWELS IN OUR ALPHABET  ",   //21
            "POETRY IS A FORM OF LITERATURE",   //22
            "HAMLET WRITTEN BY SHAKESPEARE ",   //23
            "THE HUNGER GAMES IS A TRILOGY ",   //24
            "   NERUDA WON A NOBEL PRIZE   ",   //25
            "A CALLIGRAM IS A SPECIAL POEM ",   //26
            "FIRST KINDLE LAUNCHED IN 2007 ",   //27
            "  SUN TZU MEANS 'MASTER SUN'  ",   //28
            "  STAN LEE WAS BORN IN 1922   "    //29
		}
	},

	{
		TOTAL_OF_MATH_MESSAGES,
		{
			"  PI IS AN IRRATIONAL NUMBER  ",   //00
			"A HEXAGON IS A 6-SIDED POLYGON",   //01
			"     A CUBE HAS SIX FACES     ",   //02
			"   A CUBE HAS TWELVE EDGES    ",   //03
            "A TRIANGLE HAS THREE VERTICES ",   //04
            "  A TRIANGLE HAS THREE SIDES  ",   //05
			"666 - THE NUMBER OF THE BEAST ",   //06
			"DESCARTES WAS A MATHEMATICIAN ",   //07
			"FIBONACCI WAS A MATHEMATICIAN ",   //08
			"PYTHAGORAS WAS A MATHEMATICIAN",   //09
			"  BLAISE PASCAL DIED IN 1662  ",   //10
			"  R IS USED FOR REAL NUMBERS  ",   //11
			"Q REPRESENTS RATIONAL NUMBERS ",   //12
			"   Z IS THE SET OF INTEGERS   ",   //13
			" N REPRESENTS NATURAL NUMBERS ",   //14
			"   YON IS FOUR IN JAPANESE    ",   //15
			"   CINCO IS FIVE IN SPANISH   ",   //16
			"   ACHT IS EIGHT IN GERMAN    ",   //17
			"   DIECI IS TEN IN ITALIAN    ",   //18
			"2, 4, 6, ... ARE EVEN NUMBERS ",   //19
			" 1, 3, 5, ... ARE ODD NUMBERS ",   //20
			"PI DAY IS CELEBRATED IN MARCH ",   //21
			"   X REPRESENTS THE UNKNOWN   ",   //22
			"   A MEGAGON SEEMS A CIRCLE   ",   //23
			"SOROBAN WAS DEVELOPED IN JAPAN",   //24
			"A SUAN PAN IS A CHINESE ABACUS",   //25
			"  7 MILLENNIUM PRIZE PROBLEMS ",   //26
			"A SQUARE HAS FOUR RIGHT ANGLES",   //27
			"A SQUARE HAS FOUR EQUAL SIDES ",   //28
			"    ZERO IS AN EVEN NUMBER    "    //29
		}
	}
};


static const u8* currentMessage;
static const u8* previousMessage;

static u8 nextCharacterToDisplay;
static u8 nextCharacterToHide;

static u8 delayBeforeDeleteMessage;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void displayNextCharacter ( void )
{
	const  u8 character = currentMessage[ nextCharacterToDisplay ];
	const u16 tileIndex = TILE_FONTINDEX + ( character - 0x20 );

	const  u8 positionX = 5 + nextCharacterToDisplay;
	const  u8 positionY = 3;

	VDP_setTileMapXY( MESSAGE_PLAN, MESSAGE_ATTRIBUTES( tileIndex ), positionX, positionY);

	nextCharacterToDisplay++;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void hideNextCharacter ( void )
{
	const u8 character 	= ' ';
	const u16 tileIndex = TILE_FONTINDEX + ( character - 0x20 );

	const u8 positionX 	= 5 + nextCharacterToHide;
	const u8 positionY 	= 3;

	SYS_disableInts();
	VDP_setTileMapXY( MESSAGE_PLAN, MESSAGE_ATTRIBUTES( tileIndex ), positionX, positionY);
	SYS_enableInts();

	nextCharacterToHide++;

	if ( nextCharacterToHide >= MESSAGE_MAXIMUN_CHARACTERS )
	{
		currentMessage = NULL;
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void setMessageToDisplay( const char* messageToDisplay )
{
	previousMessage = currentMessage;
	currentMessage	= messageToDisplay;

	nextCharacterToDisplay	= 0;
	nextCharacterToHide 	= 0;

	delayBeforeDeleteMessage = ( IS_PALSYSTEM ) ? 100 : 120;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void displayMessageBonus( u8 isBonusCompleted )
{
	const char *bonusMessage = ( isBonusCompleted ) ? MESSAGE_BONUS_SUCCESS : MESSAGE_BONUS_FAIL;

	setMessageToDisplay( bonusMessage );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void displayRandomMessageByTopic( u8 topicId )
{
	const u8 *messageToDisplay = NULL;
	const u8 totalOfMessagesOnThisTopic = allAvailableMessages[ topicId ].totalOfMessages;

	do
	{
		u8 messageId = random( ) % totalOfMessagesOnThisTopic;
		messageToDisplay = allAvailableMessages[ topicId ].messages[ messageId ];

	} while( previousMessage == messageToDisplay || currentMessage == messageToDisplay );

	setMessageToDisplay( messageToDisplay );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void messageReset( void )
{
	currentMessage 	= NULL;
	previousMessage = NULL;

	nextCharacterToDisplay	= 0;
	nextCharacterToHide 	= 0;

	delayBeforeDeleteMessage	= 0;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void messageUpdate( void )
{
	if ( currentMessage == NULL ) return;


	if ( nextCharacterToDisplay < MESSAGE_MAXIMUN_CHARACTERS )
	{
		displayNextCharacter( );
	}
	else
	{
		if ( delayBeforeDeleteMessage )
		{
			delayBeforeDeleteMessage--;
		}
		else
		{
			hideNextCharacter( );
		}
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
