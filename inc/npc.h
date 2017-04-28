#ifndef _NPC_H_
#define _NPC_H_


    // Public functions
    void npcReset ( void );
    void npcInitialize ( s16 positionX, s16 positionY, u8 npcType );
    void npcUpdate ( void );

    /* CHIEF NPC */
    void npcChiefInitialize ( s16 positionX, s16 positionY );
    void npcChiefUpdate ( void );

    /* BOUNCER NPC */
    void npcBouncerInitialize ( s16 positionX, s16 positionY );
    void npcBouncerUpdate ( void );

    /* GHOST NPC */
    void npcGhostInitialize ( s16 positionX, s16 positionY );
    void npcGhostUpdate ( void );

    /* CHATTER NPC */
    void npcChatterInitialize( s16 positionX, s16 positionY );
    void npcChatterUpdate( void );

    /* GEEK */
    void npcGeekInitialize( s16 positionX, s16 positionY );
    void npcGeekUpdate( void );

    /* MANBOX */
    void npcManboxInitialize( s16 positionX, s16 positionY );
    void npcManboxUpdate( void );

    /* DIBROV */
    void npcDibrovInitialize( s16 positionX, s16 positionY );
    void npcDibrovUpdate( void );


    // Types of NPCs
    #define NPC_BOUNCER     11
    #define NPC_CHIEF       12
    #define NPC_CHATTER     13
    #define NPC_MANBOX      14
    #define NPC_GEEK        15
    #define NPC_DIBROV      16
    #define NPC_GHOST       17

#endif // _NPC_H_

