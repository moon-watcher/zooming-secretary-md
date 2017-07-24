
#ifndef _SPRFACTORY_H_
#define _SPRFACTORY_H_


	typedef struct
	{
		Sprite * 	(*createCoffee)			( s16, s16 );
		Sprite * 	(*createHeart)			( s16, s16 );
		Sprite * 	(*createSecretary)		( s16, s16 );
		Sprite * 	(*createTopic)			( s16, s16 );
		Sprite * 	(*createTopicHidden)	( s16, s16 );
		Sprite * 	(*createTopicByType)	( s16, s16, u8 );
		Sprite * 	(*createPhone)			( s16, s16 );
		Sprite * 	(*createBlaBla)			( s16, s16 );
		Sprite * 	(*createChiefNPC)		( s16, s16 );
		Sprite * 	(*createBouncerNPC)		( s16, s16 );
		Sprite * 	(*createGhostNPC)		( s16, s16 );
		Sprite * 	(*createChatterNPC)		( s16, s16 );
		Sprite * 	(*createGeekNPC)		( s16, s16 );
		Sprite *    (*createManboxNPC)      ( s16, s16 );
		Sprite *    (*createDibrovNPC)      ( s16, s16 );
		Sprite *    (*createCeilingFan)     ( s16, s16 );
		Sprite *    (*createSpider)         ( s16, s16 );
		void		(*setHFlip)				( Sprite*, u8 );

	} sprFactoryStruct;


	//Public data
	extern const sprFactoryStruct SPRFactory;


	void SPRFactoryInit ( );


#endif /* _SPRFACTORY_H_ */
