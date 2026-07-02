#include "model.h"
#include <fstream>
#include <sstream>

Model::Model(const std::string filename) {

    std::ifstream file(filename);
    std::string line, prefix;

    while (file>>prefix) {
        
        if (prefix == "v") {
            vec3 v;
            file >> v.x >> v.y >> v.z;
            verts.push_back(v);
        }
        else if (prefix == "f") {
            std::string token;

            for (int i = 0; i < 3; i++) {
                file >> token;
                std::string idx_string = token.substr(0, token.find('/'));

                face_verts.push_back(stoi(idx_string)-1);
            }
        }
        else {
            // Ignore other prefixes (vt, vn, #, s) and skip the rest of the line
            std::string dummy;
            std::getline(file, dummy);
        }
    }
}
int Model::getVertCount() const {
    return verts.size();
}
int Model::getFacesCount() const {
    return face_verts.size() / 3;
}
vec3 Model::vert(const int i) const {
    return verts[i];
}
vec3 Model::vert(const int iface, const int nvert) const {
    return verts[face_verts[iface * 3 + nvert]];
}