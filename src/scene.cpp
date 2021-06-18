#include "scene.h"
#include "brdfs.h"
#define sqr2here 1.41421356237
///////////////////////////////////////////////////////////////////////////////
// Scene description.
///////////////////////////////////////////////////////////////////////////////

// NOTE: We assume the following conventions
//   X-axis: Right
//   Y-axis: Up
//   Z-axis: Back
// Note that X cross Y = Z, making this a right-handed system
/*
Scene PBR_SCENE_TWEEKEND = {
    // Red ball
    Actor {
        std::make_shared<Material>(
            Colorf { 1.0, 0.1, 0.1 }, // Color
            Colorf { 0.0, 0.0, 0.0 },  // Emission
            new path::DiffuseBRDF
        ),
        SphereGeometry {
            Vec { 1.8, 0.5, 0.0 },   // Position
            0.5                      // Radius
        }
    },

    // green ball
    Actor {
        std::make_shared<Material>(
            Colorf { 0.0, 1.0, 0.1 }, // Color
            Colorf { 0.0, 0.0, 0.0 },  // Emission
            new path::OrennayarBRDF
        ),
        SphereGeometry {
            Vec { 1.2, 0.5, 0.0 },   // Position
            0.5                     // Radius
        }
    },

    // purple ball
    Actor {
        std::make_shared<Material>(
            Colorf { 0.0, 1.0, 1.0 }, // Color
            Colorf { 0.0, 0.0, 0.0 },  // Emission
            new path::DiffuseBRDF
        ),
        SphereGeometry {
            Vec { 1.0, 0.5, 0.5 },   // Position
            0.5                     // Radius
        }
    },

    // Light 1
    Actor {
        std::make_shared<Material>(
            Colorf { 1.0, 1.0, 1.0 }, // Color
            Colorf { 6.0, 6.0, 6.0 },  // Emission
            new path::DiffuseBRDF
        ),
        SphereGeometry {
            Vec { 6.0, 4.5, -4.0 },   // Position
            3.0                      // Radius
        }
    },

    // Light 2
    Actor {
        std::make_shared<Material>(
            Colorf { 1.0, 1.0, 1.0 }, // Color
            Colorf { 6.0, 6.0, 6.0 },  // Emission
            new path::DiffuseBRDF
        ),
        SphereGeometry {
            Vec { -6.0, 4.5, -4.0 },   // Position
            3.0                      // Radius
        }
    },

    // Mirror
    Actor {
        std::make_shared<Material>(
            Colorf { 1.0, 1.0, 1.0 }, // Color
            Colorf { 0.0, 0.0, 0.0 },  // Emission
            new path::SpecularBRDF
        ),
        SphereGeometry {
            Vec { -1.5, 1.0, 0.0 },   // Position
            1.0                      // Radius
        }
    },

    // Floor
    Actor {
        std::make_shared<Material>(
            Colorf { 0.1, 1.0, 0.1 }, // Color
            Colorf { 0.0, 0.0, 0.0 },  // Emission
            new path::DiffuseBRDF
        ),
        SphereGeometry {
            Vec { 0.0, -1e5, 0.0 },  // Position
            1e5                      // Radius
        }
    }
};
*/
Scene PBR_SCENE_NEW = {
        // Red ball
        Actor {
            std::make_shared<Material>(
                Colorf { 1.0, 0.0, 0.0 }, // Color
                Colorf { 0.0, 0.0, 0.0 },  // Emission
                new path::OrennayarBRDF
            ),
            SphereGeometry {
                Vec { 1.5, 1.0, 0.0 },   // Position
                1.0                      // Radius
            }
        },

        // Light 1
        Actor {
            std::make_shared<Material>(
                Colorf { 1.0, 1.0, 1.0 }, // Color
                Colorf { 3.0, 3.0, 3.0 },  // Emission
                new path::DiffuseBRDF      
            ),
            SphereGeometry {
                Vec { 0.0, 5.0, -0.5 },   // Position
                1.0                      // Radius
            }
        },


        // yellow ball
         Actor {
             std::make_shared<Material>(
                 Colorf { 1.0, 1.0, 0.09 }, // Color
                 Colorf { 0.0, 0.0, 0.0 },  // Emission
                 new path::OrennayarBRDF
             ),
             SphereGeometry {
                 Vec { 0.0, 0.5, 0.0 },   // Position
                 0.5                      // Radius
             }
         },

        // Mirror
        Actor {
            std::make_shared<Material>(
                Colorf { 1.0, 1.0, 1.0 }, // Color
                Colorf { 0.0, 0.0, 0.0 },  // Emission
                new path::SpecularBRDF
            ),
            SphereGeometry {
                Vec { -1.5, 1.0, 0.0 },   // Position
                1.0                      // Radius
            }
        },

        // Floor
        Actor {
            std::make_shared<Material>(
                Colorf { 1.0, 1.0, 1.0 }, // Color
                Colorf { 0.0, 0.0, 0.0 },  // Emission
                new path::DiffuseBRDF
            ),
            SphereGeometry {
                Vec { 0.0, -1e5, 0.0 },  // Position
                1e5                      // Radius
            }
        },

        // Back
        Actor {
            std::make_shared<Material>(
                Colorf { 1.0, 1.0, 1.0 }, // Color
                Colorf { 0.0, 0.0, 0.0 },  // Emission
                new path::DiffuseBRDF
            ),
            SphereGeometry {
                Vec { 0.0, 0.0, -1e5 - 1.5 },  // Position
                1e5                      // Radius
            }
        },

        // Left wall
        Actor {
            std::make_shared<Material>(
                Colorf { 0.8, 0.6, 0.5 }, // Color
                Colorf { 0.0, 0.0, 0.0 },  // Emission
                new path::DiffuseBRDF
            ),
            SphereGeometry {
                Vec { -1e5 - 5, 0.0, 0.0 },  // Position
                1e5                      // Radius
            }
        },

        // Right wall
        Actor {
            std::make_shared<Material>(
                Colorf { 0.0, 0.8, 1.0 }, // Color
                Colorf { 0.0, 0.0, 0.0 },  // Emission
                new path::DiffuseBRDF
            ),
            SphereGeometry {
                Vec { 1e5 + 5, 0.0, 0.0 },  // Position
                1e5                      // Radius
            }
        },
        //r_b wall
        Actor {
            std::make_shared<Material>(
                Colorf { 0.8, 0.4, 0.1 }, // Color
                Colorf { 0.0, 0.0, 0.0 },  // Emission
                new path::DiffuseBRDF
            ),
            SphereGeometry {
                Vec { 1e5/sqr2here -2.4, 0.0, 1e5/sqr2here - 2.4 },  // Position
                1e5                      // Radius
            }
        },

        //l_b wall
        Actor {
            std::make_shared<Material>(
                Colorf { 0.0, 0.4, 0.8 }, // Color
                Colorf { 0.0, 0.0, 0.0 },  // Emission
                new path::DiffuseBRDF
            ),
            SphereGeometry {
                Vec { -1e5/sqr2here +2.4, 0.0, 1e5/sqr2here - 2.4 },  // Position
                1e5                      // Radius
            }
        },


        // Roof
        Actor {
            std::make_shared<Material>(
                Colorf { 1.0, 1.0, 1.0 }, // Color
                Colorf { 0.0, 0.0, 0.0 },  // Emission
                new path::DiffuseBRDF
            ),
            SphereGeometry {
                Vec { 0.0, 1e5 + 5, 0.0 },  // Position
                1e5                      // Radius
            }
        }
    };
    