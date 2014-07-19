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
               "dummy_level.ogg",
               "dummy_level.bmp",
               "dummy_level.wmb",
               // Dummy Level 2
               "Dummy Level 2 (Test 2)",
               "dummy_level.ogg",
               "dummy_level2.bmp",
               "test2.wmb"
      );
}

#endif /* circuit_def_h */
