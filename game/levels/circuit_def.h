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
               "Dummy Level (Test)",
               "game1_3db.wav",
               "dummy_level.bmp",
               "dummy_level.wmb",
               // Dummy Level 2
               "Dummy Level 2 (Test 2)",
               "game1_3db.wav",
               "dummy_level2.bmp",
               "test2.wmb",
               // Desert Road
               "Desert Road",
               "game1_3db.wav",
               "droad-icon.bmp",
               "droad.wmb"
      );
}

double g_circuit_skyspeeds [2] = {1, 0.25, 0.5};

float g_circuit_hdrthresholds [2] = {30, 30, 5};
float g_circuit_hdrstrengths [2] = {20, 20, 5};
float g_circuit_hdrexposures [2] = {0, 0, 0};

#endif /* circuit_def_h */
