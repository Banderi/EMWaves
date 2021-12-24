#ifndef CLION_FIELDS_H
#define CLION_FIELDS_H

//#include <math/mathc.h>
//#include <mathCPP/mathcpp.h>
#include "mathCPP/Vector2.hpp"
#include "mathCPP/Vector3.hpp"

// PARTICLE FIELDS!

#define MAX_PARTICLES_IN_FIELD 100
#define MAX_STATE_HISTORY 200
#define MAX_STATE_HISTORY_CHUNK MAX_STATE_HISTORY / 2

typedef struct particle_state {
    double lifestamp = 0;
    Vector3 position = Vector3(0,0,0);
    Vector3 velocity = Vector3(0,0,0);
    Vector3 acceleration = Vector3(0,0,0);
} particle_state;

class particle {
private:
    bool awake = false; // TODO
    particle_state state;

    particle_state history_A[MAX_STATE_HISTORY_CHUNK];
    particle_state history_B[MAX_STATE_HISTORY_CHUNK];
    int history_count = 0;

    void push_history();

public:
    void move(double newlifestamp, Vector3 newpos);
//    void accelerate(float dx, float dy, float dz);
    particle_state get_state();
    particle_state get_state(double life);
    particle_state get_signal_impingement(Vector3 point, double signal_propagation_speed);

    Vector3 get_position() const {
        return state.position;
    }
    Vector3 get_velocity() const {
        return state.velocity;
    }
    Vector3 get_acceleration() const {
        return state.acceleration;
    }

//    void wake(int id); // keep alive!!
    bool is_awake() const {
        return awake;
    }
};

class electron : public particle {
public:
    Vector3 get_E_impingement(Vector3 point, double signal_propagation_speed);
    Vector3 get_B_impingement(Vector3 point, double signal_propagation_speed);
};
class photon : public particle {
public:
    Vector3 get_E_impingement(Vector3 point, double signal_propagation_speed);
    Vector3 get_B_impingement(Vector3 point, double signal_propagation_speed);
    void get_frequency();
    void get_wavelength();
    void get_momentum();
    void get_energy();
};

class field_system {
public:
    electron g_electrons[MAX_PARTICLES_IN_FIELD] = {};
    int electrons_count = 0;

    photon g_photons[MAX_PARTICLES_IN_FIELD] = {};
    int photon_count = 0;

    static Vector3 get_E_impingement(Vector3 point, double signal_propagation_speed);

};

#ifdef __cplusplus
#define CEXTERN extern "C"
#else
#define CEXTERN
#endif

CEXTERN field_system Fields;


#endif //CLION_FIELDS_H
