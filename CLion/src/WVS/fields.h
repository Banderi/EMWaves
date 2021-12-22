#ifndef CLION_FIELDS_H
#define CLION_FIELDS_H

// PARTICLE FIELDS!

class particle {
public:
    struct {
        float x;
        float y;
        float z;
    } position;
    struct {
        float x;
        float y;
        float z;
    } velocity;

    ////

    void move(float dx, float dy, float dz);
//    void accelerate(float dx, float dy, float dz);

};

class electron : public particle {
    void get_E_impingement();
    void get_B_impingement();
};
class photon : public particle {
    void get_E_impingement();
    void get_B_impingement();
    void get_frequency();
    void get_wavelength();
    void get_momentum();
    void get_energy();
};

class field_system {
public:
    electron g_electrons[100];
    electron g_photons[100];

    field_system() {
        g_electrons[25].position.x = 24.7;
    }

};

#ifdef __cplusplus
#define CEXTERN extern "C"
#else
#define CEXTERN
#endif

CEXTERN field_system Fields;


#endif //CLION_FIELDS_H
