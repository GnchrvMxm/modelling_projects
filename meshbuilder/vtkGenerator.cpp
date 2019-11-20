#pragma once

#include <typeinfo>
#include <fstream>

#define CELL_TYPE_TRIANGLE 5
#define CELL_TYPE_RECTANGLE 9
#define BC_TYPE -1

using namespace std;

template<typename T1,typename T2>
class vtkGenerator
{
    ofstream out;

public:

    vtkGenerator(const char* filename)
    {
        out.open(filename,std::fstream::out);
    }

    void printInVTK(double pointsNumber,const FixedSizeMeshContainer<T1>& coords,const VariableSizeMeshContainer<T2>& topoEN)
    {
        out << "# vtk DataFile Version 2.0" << endl;
        out << "Generated by Maxkile's vtkGenerator" << endl;
        out << "ASCII" << endl;
        out << "DATASET UNSTRUCTURED_GRID" << endl;
        out << endl;
        const char* type;
        if (!strcmp(typeid(pointsNumber).name(),"i"))
        {
            type = "int";
        }
        else if (!strcmp(typeid(pointsNumber).name(),"d"))
        {
            type = "float";
        }

        out << "POINTS " << pointsNumber << " " <<  type  << endl;

        coords.printContainer(out); 

        out << "CELLS " << topoEN.getBlockNumber() << " " << topoEN.getTotalSize() + topoEN.getBlockNumber() << endl;

        topoEN.printContainer(out);

        out << "CELL_TYPES " << topoEN.getBlockNumber() << endl;

        for (int i = 0; i < topoEN.getBlockNumber();++i)
        {
            switch(topoEN.getBlockSize(i))
            {
                case 3: out << CELL_TYPE_TRIANGLE << " ";
                        break;
                case 4: out << CELL_TYPE_RECTANGLE << " ";
                        break;
            }
        }

        out << endl;

        out << "POINT_DATA " << pointsNumber << endl;

        out << "SCALARS BCType " << type << " " << 1 << endl;

        out << "LOOKUP_TABLE " << "default" << endl;

        for (int i = 0; i < pointsNumber;++i)
        {
            out << BC_TYPE << " ";
        }

        out << endl;
    }

    ~vtkGenerator()
    {
        out.close();
    }
};
