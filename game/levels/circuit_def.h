#ifndef circuit_def_h
#define circuit_def_h

// repeated:
// TITEL ("Super Lotti Course")
// MUSIKDATEINAME ("course_lotti.ogg")
// BILDDATEINAME ("course_lotti.bmp")
// LEVELDATEINAME ("course_lotti.wmb")

#define CIRCUIT_DEF_ENTRIES 4

TEXT* g_circuits = {
      string (
               // Dummy Level
               "Ackplains",
               "game1_3db.wav",
               "dummy_level2.bmp",
               "test2.wmb",
               // Desert Road
               "Desert Road",
               //"game1_3db.wav",
               "POL_game_looped.wav",
               "droad-icon.bmp",
               "droad.wmb",
               // King Lotti's Bridge
               "King Lotti's Bridge",
               "game1_3db.wav",
               "kbridge.bmp",
               "kbridge.wmb",
               // Icenex
               "Icenex",
               "game1_3db.wav",
               "icenex.bmp",
               "icenex.wmb"
      );
}

double g_circuit_skyspeeds [2] = {0.25, 0.75};

float g_circuit_hdrthresholds [4] = {20, 55, 55, 30};
float g_circuit_hdrstrengths [4] = {20, 50, 50, 10};
float g_circuit_hdrexposures [4] = {0, 0, 0, 0};

#endif /* circuit_def_h */
