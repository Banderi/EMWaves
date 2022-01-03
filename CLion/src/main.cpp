#include <gdnative_api_struct.gen.h>
#include <cstring>
#include "main.h"
#include "WVS/fields.h"

const char *p_class_name = "WAVES";
const godot_gdnative_core_api_struct *API = nullptr;
const godot_gdnative_ext_nativescript_api_struct *nativescript_API = nullptr;

typedef struct {
    char data[256];
} globals_struct;

void *init_globals() {
    auto global_data = (globals_struct*)API->godot_alloc(sizeof(globals_struct));
    strcpy(global_data->data, "World from GDNative!");
    return (void*)global_data;
}

///////// function overrides for default args
void register_method(const char *p_name, GDCALLINGCONV godot_variant (*method)(godot_object *, void *, void *, int, godot_variant **)) {
    return register_method(p_name, method, GODOT_METHOD_RPC_MODE_DISABLED);
}

// empty objects
godot_variant empty_variant() {
    godot_variant ret;
    return ret;
}
godot_array empty_array() {
    godot_array arr;
    API->godot_array_new(&arr);
    return arr;
}

// these are simple variable types. they don't require deallocation
godot_variant to_variant(int a) {
    godot_variant ret;
    API->godot_variant_new_int(&ret, a);
    return ret;
}
godot_variant to_variant(bool a) {
    godot_variant ret;
    API->godot_variant_new_bool(&ret, a);
    return ret;
}
godot_variant to_variant(double a) {
    godot_variant ret;
    API->godot_variant_new_real(&ret, a);
    return ret;
}
godot_variant to_variant(godot_vector3 a) {
    godot_variant ret;
    API->godot_variant_new_vector3(&ret, &a);
    return ret;
}

// destroy complex variant/object types
void free(godot_variant a) {
    API->godot_variant_destroy(&a);
}
void free(godot_array a) {
    API->godot_array_destroy(&a);
}
void free(godot_string a) {
    API->godot_string_destroy(&a);
}
void free(godot_object *a) {
    API->godot_object_destroy(&a);
}

// from Godot to C types -- these take variants as input, they REQUIRE deallocation!!
int to_int(godot_variant a, bool dealloc = true) {
    auto ret = API->godot_variant_as_int(&a);
    if (dealloc)
        free(a);
    return ret;
}
bool to_bool(godot_variant a, bool dealloc = true) {
    auto ret = API->godot_variant_as_bool(&a);
    if (dealloc)
        free(a);
    return ret;
}
double to_double(godot_variant a, bool dealloc = true) {
    auto ret = API->godot_variant_as_real(&a);
    if (dealloc)
        free(a);
    return ret;
}

// these are complex variant/object types -- they REQUIRE deallocation!!
godot_variant to_variant_unsafe(godot_string a, bool dealloc = true) {
    godot_variant ret;
    API->godot_variant_new_string(&ret, &a);
    if (dealloc)
        free(a);
    return ret;
}
godot_variant to_variant_unsafe(const char *a) {
    // first, declare, initialize and fill a string object
    godot_string str;
    API->godot_string_new(&str);
    API->godot_string_parse_utf8(&str, a);

    // then, transfer to variant
    godot_variant ret;
    API->godot_variant_new_string(&ret, &str);

    // remember to deallocate the object!
//    if (dealloc)
        free(str);
    return ret;
}
godot_variant to_variant_unsafe(godot_array a, bool dealloc = true) {
    godot_variant ret;
    API->godot_variant_new_array(&ret, &a);
    if (dealloc)
        free(a);
    return ret;
}
godot_variant to_variant_obj(godot_object *a, bool dealloc = true) {
    godot_variant ret;
    API->godot_variant_new_object(&ret, a);
    if (dealloc)
        free(a);
    return ret;
}

// these construct Godot arrays
godot_array to_array(godot_variant a) {
    return API->godot_variant_as_array(&a);
}
void array_push_back(godot_array *arr, const godot_variant a, bool dealloc = true) {
    API->godot_array_push_back(arr, &a);
    if (dealloc)
        free(a);
}
godot_array constr_godot_array(godot_variant variants[], int num) {
    godot_array arr;
    API->godot_array_new(&arr);
    for (int i = 0; i < num; ++i)
        API->godot_array_push_back(&arr, &variants[i]);
    return arr;
}
godot_array constr_godot_array(godot_variant **variants, int num) {
    godot_array arr;
    API->godot_array_new(&arr);
    for (int i = 0; i < num; ++i)
        API->godot_array_push_back(&arr, variants[i]);
    return arr;
}
godot_variant get_param(int param, godot_variant **p_args, int p_num_args) {
    godot_variant ret;
    if (p_num_args == 0) // no parameters!
        return ret;
    godot_array arr = constr_godot_array(p_args, p_num_args);

    ret = API->godot_array_get(&arr, param);
    free(arr);
    return ret;
}

// these construct Godot & CPP vector3 objects
godot_vector3 to_vector3(godot_real x, godot_real y, godot_real z) {
    godot_vector3 vec;
    API->godot_vector3_new(&vec, x, y, z);
    return vec;
}
godot_vector3 to_vector3(godot_variant x, godot_variant y, godot_variant z) {
    godot_vector3 vec;
    API->godot_vector3_new(&vec, to_double(x), to_double(y), to_double(z));
    return vec;
}
godot_vector3 to_vector3(godot_variant a, bool dealloc = true) {
    auto ret = API->godot_variant_as_vector3(&a);
    if (dealloc)
        free(a);
    return ret;
}
godot_vector3 to_vector3(Vector3 a) {
    return to_vector3(a.X, a.Y, a.Z);
}
Vector3 to_Vector3(godot_vector3 a) {
    double X = API->godot_vector3_get_axis(&a, GODOT_VECTOR3_AXIS_X);
    double Y = API->godot_vector3_get_axis(&a, GODOT_VECTOR3_AXIS_Y);
    double Z = API->godot_vector3_get_axis(&a, GODOT_VECTOR3_AXIS_Z);
    return Vector3(X, Y, Z);
}
Vector3 to_Vector3(godot_variant a) {
    auto var = to_vector3(a);
    double X = API->godot_vector3_get_axis(&var, GODOT_VECTOR3_AXIS_X);
    double Y = API->godot_vector3_get_axis(&var, GODOT_VECTOR3_AXIS_Y);
    double Z = API->godot_vector3_get_axis(&var, GODOT_VECTOR3_AXIS_Z);
    return Vector3(X, Y, Z);
}

godot_variant debug_line_text(const char *text, int num) {
    godot_array arr = empty_array();
    array_push_back(&arr, to_variant_unsafe(text));
    array_push_back(&arr, to_variant(num));
    return to_variant_unsafe(arr);
}

//////////// NATIVESCRIPT CLASS MEMBER FUNCTIONS

godot_variant get_test_string(godot_object *p_instance, void *p_method_data, void *p_globals, int p_num_args, godot_variant **p_args) {

    godot_string data;
    godot_variant ret;

    API->godot_string_new(&data);
    API->godot_string_parse_utf8(&data, "test");
    API->godot_variant_new_string(&ret, &data);
    API->godot_string_destroy(&data);

    return ret;
//    return to_variant(((globals_struct *)(p_globals))->data);
}
godot_variant get_two(godot_object *p_instance, void *p_method_data, void *p_globals, int p_num_args, godot_variant **p_args) {

    // this DOES NOT create a leak...
//    godot_string data;
//    godot_variant ret;
//
//    API->godot_string_new(&data);
//    API->godot_string_parse_utf8(&data, "test");
//    API->godot_variant_new_string(&ret, &data);
//    API->godot_string_destroy(&data);
//    return ret;


    // .....................


    // this DOES NOT create a leak...
//    godot_variant ret;
//    for (int i = 0; i < 20; ++i) {
//        godot_string data;
//        API->godot_string_new(&data); // <--- this leaves behind a "phantom" string, but we already destroy it at the end of each loop.
//        API->godot_string_parse_utf8(&data, "test");
//
//        if (i > 0) // only destroy after the first loop - you can not destroy uninitialized objects!
//            API->godot_variant_destroy(&ret); // DESTROY the variant first if already initialized!!!!
//        API->godot_variant_new_string(&ret, &data); // <--- this leaves behind a "phantom" variant, so it needs to be destroyed!!
//
//        API->godot_string_destroy(&data);
//    }


    // .....................


    // this DOES NOT create a leak...
    auto arr = empty_array();
    for (int i = 0; i < 20; ++i) {
        auto b = empty_array();
        array_push_back(&b, to_variant(i));
        array_push_back(&arr, to_variant_unsafe(b));
    }

    return to_variant_unsafe(arr);
}
godot_variant get_heartbeat(godot_object *p_instance, void *p_method_data, void *p_globals, int p_num_args, godot_variant **p_args) {
    godot_array arr = constr_godot_array(p_args, p_num_args);
    return to_variant_unsafe(arr);
}

godot_variant get_particle_test(godot_object *p_instance, void *p_method_data, void *p_globals, int p_num_args, godot_variant **p_args) {
//    auto p = Fields.g_electrons[25];
//    auto a = to_variant(p.position.x);
//    return a;


//    struct vec3 test;
//    vec3(&test, 5.0, 6.0, 7.0);

//    auto vec = vector3
//    Vector3 vec3 = Vector3(5, 7, 8);

    return to_variant(to_vector3(1, 2, 3));
//    return to_variant(to_vector3(vec3));
}

godot_variant load_electron_state(godot_object *p_instance, void *p_method_data, void *p_globals, int p_num_args, godot_variant **p_args) {
    godot_variant g_electron_id = get_param(0, p_args, p_num_args);
    godot_variant g_electron_life = get_param(1, p_args, p_num_args);
    godot_variant g_electron_pos = get_param(2, p_args, p_num_args);
    godot_variant g_electron_vel = get_param(2, p_args, p_num_args);

    int electron_id = to_int(g_electron_id);
    double electron_life = to_double(g_electron_life);
    Vector3 electron_position = to_Vector3(g_electron_pos);
    Vector3 electron_velocity = to_Vector3(g_electron_vel);

    if (electron_id < MAX_PARTICLES_IN_FIELD) {
//        Fields.g_electrons[electron_id].wake(electron_id);
        Fields.g_electrons[electron_id].move(electron_life, electron_position);
    }

    Vector3 safety_check_return = Fields.g_electrons[electron_id].get_position();
    return to_variant(electron_life);
}
godot_variant get_electron_state(godot_object *p_instance, void *p_method_data, void *p_globals, int p_num_args, godot_variant **p_args) {
    godot_variant g_electron_id = get_param(0, p_args, p_num_args);
    godot_variant g_electron_life = get_param(1, p_args, p_num_args);

    int electron_id = to_int(g_electron_id);
    double electron_life = to_double(g_electron_life);

    particle_state state;
    if (electron_id < MAX_PARTICLES_IN_FIELD) {
        if (electron_life == -1)
            state = Fields.g_electrons[electron_id].get_state();
        else
            state = Fields.g_electrons[electron_id].get_state(electron_life);
    }

    return to_variant(to_vector3(state.velocity));
}

godot_variant get_E_at_point(godot_object *p_instance, void *p_method_data, void *p_globals, int p_num_args, godot_variant **p_args) {
    godot_variant g_probe_point = get_param(0, p_args, p_num_args);
    godot_variant g_speed_of_light = get_param(1, p_args, p_num_args);

    Vector3 probe_point = to_Vector3(g_probe_point);
    double speed_of_light = to_double(g_speed_of_light);

    Vector3 E = Fields.get_E_impingement(probe_point, speed_of_light);

    return to_variant(to_vector3(E));
}


void init_nativescript_methods() {

    // register methods
    register_method("get_test_string", &get_test_string);
    register_method("get_two", &get_two);
    register_method("get_heartbeat", &get_heartbeat);

    //

    register_method("get_particle_test", &get_particle_test);

    register_method("load_electron_state", &load_electron_state);
    register_method("get_electron_state", &get_electron_state);

    register_method("get_E_at_point", &get_E_at_point);
}