#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H

#include "../clover.h"

#define N 300
#define M 100

class voxel_grid{
    public:
        voxel_grid(){}
        
        void init(hittable_list& world){
            hittable* obj;
            for(int i=-N; i<N; i++){
                for(int j=-N; j<N; j++){
                    for(int k=-N; k<N; k++){
                        grid[i+N][j+N][k+N] = world.dist(point3((double)i/M,(double)j/M,(double)k/M), obj);
                    }
                }
            }
        }

        void read(const char* path) {
            freopen("hello world SDF.txt", "w", stdin);
            for(int i=-N; i<N; i++){
                for(int j=-N; j<N; j++){
                    for(int k=-N; k<N; k++){
                        std::cin>>grid[i+N][j+N][k+N];
                    }
                }
            }
        }

        void write() {
            freopen("SDF.txt", "w", stdout);
            for(int i=-N; i<N; i++){
                for(int j=-N; j<N; j++){
                    for(int k=-N; k<N; k++){
                        std::cout<<grid[i+N][j+N][k+N]<<"\n";
                    }
                }
            }
            freopen("image.ppm", "w", stdout);
        }

        double at(const point3& p) const {
            if(abs(M*p.x())>N-2 || abs(M*p.y())>N-2 || abs(M*p.z())>N-2)
                return infinity;

            int x = M * p.x() + N, 
                y = M * p.y() + N, 
                z = M * p.z() + N;
            double a = M * p.x() + N - x,
                   b = M * p.y() + N - y,
                   c = M * p.z() + N - z;

            // debug use
            // std::cerr<<x<<" "<<y<<" "<<z<<"\n";
            // std::cerr<<grid[x+1][y+1][z+1]<<" "
            //          <<grid[x+1][y+1][z]<<" "
            //          <<grid[x+1][y][z+1]<<" "
            //          <<grid[x+1][y][z]<<" "
            //          <<grid[x][y+1][z+1]<<" "
            //          <<grid[x][y+1][z]<<" "
            //          <<grid[x][y][z+1]<<" "
            //          <<grid[x][y][z]<<"\n";

            return a*b*c*grid[x+1][y+1][z+1]
                 + a*b*(1-c)*grid[x+1][y+1][z]
                 + a*(1-b)*c*grid[x+1][y][z+1]
                 + a*(1-b)*(1-c)*grid[x+1][y][z]
                 + (1-a)*b*c*grid[x][y+1][z+1]
                 + (1-a)*b*(1-c)*grid[x][y+1][z]
                 + (1-a)*(1-b)*c*grid[x][y][z+1]
                 + (1-a)*(1-b)*(1-c)*grid[x][y][z];
        }

    public:
        double grid[2*N][2*N][2*N];
};

// voxel grid with z<0
// class voxel_grid{
//     public:
//         voxel_grid(){}
        
//         void init(hittable_list& world){
//             hittable* obj;
//             for(int i=-N; i<N; i++){
//                 for(int j=-N; j<N; j++){
//                     for(int k=0; k<N; k++){
//                         grid[i+N][j+N][k] = world.dist(point3((double)i/M,(double)j/M,(double)-k/M), obj);
//                     }
//                 }
//             }
//         }

//         double at(const point3& p) const {
//             if(abs(M*p.x())>N-2 || abs(M*p.y())>N-2 || abs(M*p.z())>N-2 || p.z()>0)
//                 return infinity;

//             int x = M * p.x() + N, 
//                 y = M * p.y() + N, 
//                 z = M * (-p.z());
//             double a = M * p.x() + N - x,
//                    b = M * p.y() + N - y,
//                    c = M * (-p.z()) - z;
//             //std::cerr<<p.x()<<" "<<p.y()<<" "<<p.z()<<"\n";
//             //std::cerr<<x<<" "<<y<<" "<<z<<"\n";
//             //std::cerr<<a<<" "<<b<<" "<<c<<"\n";
//             /*std::cerr<<grid[x][y][z]<<" "<<grid[x][y][z+1]<<" "<<
//                     grid[x][y+1][z]<<" "<<grid[x][y+1][z+1]<<" "<<
//                     grid[x+1][y][z]<<" "<<grid[x+1][y][z+1]<<" "<<
//                     grid[x+1][y+1][z]<<" "<<grid[x+1][y+1][z+1]<<"\n";*/
//             return a*b*c*grid[x+1][y+1][z+1]
//                  + a*b*(1-c)*grid[x+1][y+1][z]
//                  + a*(1-b)*c*grid[x+1][y][z+1]
//                  + a*(1-b)*(1-c)*grid[x+1][y][z]
//                  + (1-a)*b*c*grid[x][y+1][z+1]
//                  + (1-a)*b*(1-c)*grid[x][y+1][z]
//                  + (1-a)*(1-b)*c*grid[x][y][z+1]
//                  + (1-a)*(1-b)*(1-c)*grid[x][y][z];
//         }

//     public:
//         double grid[2*N][2*N][N];
// };

#endif