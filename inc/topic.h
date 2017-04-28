#ifndef _TOPIC_H_
#define _TOPIC_H_


    // Maximum of different topics
    #define TOPIC_MAX               4

    // Kinds of frames for each animation
    #define TOPIC_FRAME_VISIBLE     0
    #define TOPIC_FRAME_HIDDEN      1
    #define TOPIC_FRAME_BLINK       2

    // Kinds of topics
    #define TOPIC_TYPE_EARTH        0
    #define TOPIC_TYPE_HISTORY      1
    #define TOPIC_TYPE_LITERATURE   2
    #define TOPIC_TYPE_SPACE        3
    #define TOPIC_TYPE_BONUS        4
 	#define TOPIC_TYPE_NONE			5

    //Public functions
    void topic_reset( void );
    void topic_init( s16 x, s16 y, u8 type );
    void topic_step( void );
    void setRandomActiveTopic( void );
    u8 topic_checkCollision( s16 spr_x, s16 spr_xx, s16 spr_y, s16 spr_yy );
    u8 getActiveTopicType( void );
    u8 getRandomTopic( void );


#endif // _TOPIC_H_
