#include <iostream>
#include <algorithm>
#include <fstream>
//#include <iomanip> ���� �� ����� �������� ����� ������ ���� � �������
//������������ �������� ���� = 100
#include <cmath>

#define INF 101

using namespace std;

template<typename T>
void printMatrix(T** matrix, int numberOfVert, int OrderFlag=0) //int OrderFlag - ���� ��������� �������� � �������. 0 � ����; 1 - � �������(����� ��������� 1 � ������� ��������).
{
    double CtrlSum=0; //����������� �����
    for(int i = 0; i < numberOfVert; i++) {
        for(int j = 0; j < numberOfVert; j++)
        {
            cout.width(7);
            cout << matrix[i][j]+OrderFlag<< " ";
            CtrlSum+=matrix[i][j]+OrderFlag; //��������� � 1
        }
        cout << endl;
    }
    cout << "CtrlSum=" << CtrlSum<<endl;
}
template<typename T>
void printMatrixToFile(char* filename, T** matrix, int numberOfVert, int OrderFlag=0) //int OrderFlag - ���� ��������� �������� � �������. 0 � ����; 1 - � �������(����� ��������� 1 � ������� ��������).
{
    ofstream ofile(filename);
    if(!filename) cout <<"Cannot open file " << filename << endl;

    double CtrlSum=0; //����������� �����
    for(int i = 0; i < numberOfVert; i++) {
        for(int j = 0; j < numberOfVert; j++)
        {
            ofile<<std::fixed;
        //    cout.width(7);
           // cout << matrix[i][j]+OrderFlag<< " ";
            ofile<<matrix[i][j]+OrderFlag<< " ";
            CtrlSum+=matrix[i][j]+OrderFlag; //��������� � 1
            ofile.unsetf(ios::fixed | ios::scientific);
        }
     //   cout << endl;
        ofile <<endl;
    }
  //  cout << "CtrlSum=" << CtrlSum<<endl;
    ofile<<std::fixed;
    ofile << "CtrlSum=" << CtrlSum<<endl;
    ofile.unsetf(ios::fixed | ios::scientific);
    ofile.close();
   // cout << std::resetiosflags(std::ios::fixed); //���� �� ����� �������� ����� ������ ���� � �������
}
//matrix - ������� ���������
void originalFloydWarshall(double **matrix, int numberOfVert, int **MPath) { //double **matrix-������� �������� ����������, ���� �� ������� ���������, int numberOfVert-����� ������, int **MPath-������� �������������
  //  int MPath[numberOfVert][numberOfVert]; //������� 1-�� �������� � ���� �� i �� j;  �.�. ���� �� i �� j �����: i->MPath[i][j]->...->j
    for(int i=0; i< numberOfVert; i++) //��������� ������� ���������� ����������.
    {
        for(int j=0; j< numberOfVert; j++)
        {
            MPath[i][j]=j;
        }
    }
    //����������� �� ���� �������� � ���� ����� �������� ����
    //����� ������� k
    for(int k = 0; k < numberOfVert; k++) {
        for(int i = 0; i < numberOfVert; i++) {
            for(int j = 0; j < numberOfVert; j++) {
                //����� �������� ����� ����� ������������ ����� ������
                //� ������ ����� i <-> k + k <-> j (���� ����� k ������ �������)
                if(matrix[i][j]>matrix[i][k]+matrix[k][j])
                {
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                    MPath[i][j]=MPath[i][k];
                }
             //   matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);

            }
        }
    }
//    int CtrlSum=0; //����������� �����
  //  cout << "MPath[i][j]:" <<endl;
 //   for(int i=0; i< numberOfVert;i++)
//    {
 //       for(int j=0;j< numberOfVert; j++)
  //      {
  //          cout << MPath[i][j]+1 << " "; //��������� � 1
   //         CtrlSum+=MPath[i][j]+1; //��������� � 1
   //     }
   //     cout <<endl;
  //  }
 //   cout << "CtrlSum=" << CtrlSum<<endl;
    return;
}
void EdgeLoad(double **MNodeToNodeLoad, int **MPath, int numberOfVert, double **MEdgeLoad)
{
    int i, j;
    for(i=0; i<numberOfVert; i++)
    {
        for(j=0; j<numberOfVert; j++)
        {
            int k1=i;
            int k2=MPath[i][j]; //
            MEdgeLoad[k1][k2]+=MNodeToNodeLoad[i][j];
            while(k2!=j)
            {
                k1=k2;
                k2=MPath[k2][j];
                MEdgeLoad[k1][k2]+=MNodeToNodeLoad[i][j];
            }
        }
    }
}
void ThreatNumber(double **MEdgeLoad, int numberOfVert, double p0, int **MThreatNumber)
{
    int i, j;
/*    int k; //����� ��������������� � long double. long long �� ��������. */
    long double p, numerator, sum;
    for(i=0; i<numberOfVert; i++)
    {
        for(j=0;j<numberOfVert;j++)
        {
            if(MEdgeLoad[i][j]>0)
            {
        /*        MThreatNumber[i][j]=1; //��� ��������. ������, ��� 2 ��������. ���, ������� � ������������, ������� � ���. ��� ���������� ���������� �� ����������� 2�� �������� � �������� �������. ���� ��������, ��� 2 ������� ������, �.�. ��� ��� ������������� ������ �������� � �������� �������
                sum=0;
                for(k=0; k<MThreatNumber[i][j]; k++) //���� ����������� �������
                {
                    sum+=pow((long double)MEdgeLoad[i][j], k)/tgamma((long double)k+1); //MEdgeLoad ���� ����� ��������������� � long double, ����� pow ���������� �������������.
                }
                p=pow((long double)MEdgeLoad[i][j], MThreatNumber[i][j])/tgamma((long double)MThreatNumber[i][j]+1)/sum; */
                MThreatNumber[i][j]=1;
                numerator=MEdgeLoad[i][j];
                sum=1+MEdgeLoad[i][j];
                p=numerator/sum;
                while(p>p0)
                {
        /*          MThreatNumber[i][j]++; //��� ��������
                    sum=0;
                 //   cout<< "sum:"<<endl;
                    for(k=0; k<MThreatNumber[i][j]; k++) //���� ����������� �������
                    {
                        sum+=pow((long double)MEdgeLoad[i][j], k)/tgamma((long double)k+1);
                  //      cout << sum <<endl;
                    }
               //     cout<< "sum="<<sum<<endl;
               //     cout<< "pow((long double)MEdgeLoad[i][j], MThreatNumber[i][j])/tgamma((long double)MThreatNumber[i][j]+1)=" << pow((long double)MEdgeLoad[i][j], MThreatNumber[i][j])/tgamma((long double)MThreatNumber[i][j]+1)<< endl;
                    p=pow((long double)MEdgeLoad[i][j], MThreatNumber[i][j])/tgamma((long double)MThreatNumber[i][j]+1)/sum;
        */
                    MThreatNumber[i][j]++;
                    numerator=numerator*MEdgeLoad[i][j]/MThreatNumber[i][j];
                    sum=sum+numerator;
                    p=numerator/sum;
                }
            }
            else MThreatNumber[i][j]=0;
         //   break;
        }
     //   break;
    }
//    cout <<"tgamma((long double)1000+1)="<< tgamma((long double)1000+1) <<endl;
 //   cout <<"tgamma((long double)MThreatNumber[i][j]+1)="<< tgamma((long double)MThreatNumber[i][j]+1) <<endl;
}
void ThreatNumberBruteForce(double **MEdgeLoad, int numberOfVert, double p0, int **MThreatNumber)
{
    int i, j;
    int k; //����� ���������������� � long double. long long �� ��������.
    long double p, sum;
    for(i=11; i<numberOfVert; i++)
    {
        for(j=11;j<numberOfVert;j++)
        {
            if(MEdgeLoad[i][j]>0)
            {
                MThreatNumber[i][j]=1; //��� ��������. ������, ��� 2 ��������. ���, ������� � ������������, ������� � ���. ��� ���������� ���������� �� ����������� 2�� �������� � �������� �������. ���� ��������, ��� 2 ������� ������, �.�. ��� ��� ������������� ������ �������� � �������� �������
                sum=0;
                for(k=0; k<MThreatNumber[i][j]; k++) //���� ����������� �������
                {
                    sum+=pow((long double)MEdgeLoad[i][j], k)/tgamma((long double)k+1); //MEdgeLoad ���� ����� ��������������� � long double, ����� pow ���������� �������������.
                }
                p=pow((long double)MEdgeLoad[i][j], MThreatNumber[i][j])/tgamma((long double)MThreatNumber[i][j]+1)/sum;
                while(p>p0)
                {
                MThreatNumber[i][j]++; //��� ��������
                    sum=0;
                 //   cout<< "sum:"<<endl;
                    for(k=0; k<MThreatNumber[i][j]; k++) //���� ����������� �������
                    {
                        sum+=pow((long double)MEdgeLoad[i][j], k)/tgamma((long double)k+1);
                  //      cout << sum <<endl;
                    }
               //     cout<< "sum="<<sum<<endl;
               //     cout<< "pow((long double)MEdgeLoad[i][j], MThreatNumber[i][j])/tgamma((long double)MThreatNumber[i][j]+1)=" << pow((long double)MEdgeLoad[i][j], MThreatNumber[i][j])/tgamma((long double)MThreatNumber[i][j]+1)<< endl;
                    p=pow((long double)MEdgeLoad[i][j], MThreatNumber[i][j])/tgamma((long double)MThreatNumber[i][j]+1)/sum;
                     cout <<"p 11x11="<< p <<endl;
                }
            }
            else MThreatNumber[i][j]=0;
            break;
        }
        break;
    }

 //   cout <<"tgamma((long double)MThreatNumber[i][j]+1)="<< tgamma((long double)MThreatNumber[i][j]+1) <<endl;
}
/*  7 �����. ������� ������������� ������� �� ��� ����� �����*/
void TrafficIntensity(int **MThreatNumber, int numberOfVert, double a0, double **MTrafficIntensity)
{
    int i, j;
    for(i=0; i< numberOfVert; i++)
    {
        for(j=0; j<numberOfVert; j++)
        {
            MTrafficIntensity[i][j]=MThreatNumber[i][j]*a0;
        }
    }
}
/*  8 �����. ������� ���������� ����������� ����� �����*/
void CommunLineCapacity(double **MTrafficIntensity, int numberOfVert, double L, double T0, double **MCommunLineCapacity)
{
    int i, j;
    for(i=0; i< numberOfVert; i++)
    {
        for(j=0; j< numberOfVert; j++)
        {
            if(MTrafficIntensity[i][j]!=0)
            {
                MCommunLineCapacity[i][j]=MTrafficIntensity[i][j]+L/T0;
            }
            else
            {
                MCommunLineCapacity[i][j]=0;
            }
        }
    }
}
/* 9 �����. ������� MO*/
void OCalc(double **MB0, double **MTrafficIntensity, int **MPath, int numberOfVert, double L, double Topt, double dc, double **MO) //��������� � MO. ��������� ��������� �� ��������.
{
    //MDel ������� �������� Tij �� ������ �����
    double **MDel=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MDel[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    //MDl ������� �������� dlij ����� ������ �� ���������.
    double **MDl=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MDl[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    for(int iO=0; iO< numberOfVert; iO++)
    {
        for(int jO=0; jO<numberOfVert; jO++) //� ����� ���� ������ �������� MO[iO][jO]
        {
            //��������� MDel
            for(int i=0; i< numberOfVert; i++)
            {
                for(int j=0; j<numberOfVert; j++)
                {
                    if(MB0[i][j]>0) //���� ����� ������������ � ���������
                    {
                        if(i==iO && j==jO)
                        {
                            MDel[i][j]=L*8/(MB0[i][j]+dc-MTrafficIntensity[i][j]);
                        }
                        else
                        {
                            MDel[i][j]=L*8/(MB0[i][j]-MTrafficIntensity[i][j]); //��� ����� ������� ��� �����
                        }
                    }
                    else
                    {
                        MDel[i][j]=0;
                    }
                }
            }
     //       cout<<"MDel:"<<endl;
       //     printMatrix(MDel, numberOfVert);
            //��������� MDl
            for(int i=0; i<numberOfVert; i++)
            {
                for(int j=0; j<numberOfVert; j++) //��������� �������� ��� ������� ��������
                {
                    int k1=i;
                    int k2=MPath[i][j];
                    MDl[i][j]=0;        //�������������
                    MDl[i][j]+=MDel[k1][k2];
                    while(k2!=j)
                    {
                        k1=k2;
                        k2=MPath[k2][j];
                        MDl[i][j]+=MDel[k1][k2];
                    }
                }
            }
    //        cout<<"MDl:"<<endl;
     //       printMatrix(MDl, numberOfVert);
            //��������� MO[iO][jO]
            MO[iO][jO]=0;
            for(int i=0; i<numberOfVert; i++)
            {
                for(int j=0; j<numberOfVert; j++)
                {
                    if(MB0[iO][jO]!=0)
                    {
                        MO[iO][jO]+=(MDl[i][j]-Topt)*(MDl[i][j]-Topt);
                    }
                    else
                    {
                        MO[iO][jO]=0;
                    }
                }
            }

    //    break;
        }
    //    break;
    }
    for(int i=0; i< numberOfVert; i++)
    {
        free(MDel[i]);
        free(MDl[i]);
    }
    free(MDel);
    free(MDl);
}
/* ���� OminCurrent*/
void OminCurrentCalc(double **MO, int numberOfVert, int *iOmin, int *jOmin, double *OminCurrent)
{
    *OminCurrent=MO[0][0];
    *iOmin=0;
    *jOmin=0;
    for(int iO=0; iO<numberOfVert; iO++)
    {
        for(int jO=0; jO<numberOfVert; jO++)
        {
            if(MO[iO][jO]< *OminCurrent && MO[iO][jO]>0) //���� ��������� ����, ��� MO[iO][jO] ������ ����� 0;
            {
                *OminCurrent=MO[iO][jO];
                *iOmin=iO;
                *jOmin=jO;
            }
        }
    }
}
bool IsGreaterThen(double **MDl, int numberOfVert, double Topt) // ���� ���� �� ���� ������� ������ ��������� �������, �� ������������ true ����� false
{
    int i, j;
    for(i=0;i<numberOfVert; i++)
    {
        for(j=0; j<numberOfVert; j++)
        {
            if(MDl[i][j]>Topt) return true;
        }
    }
    return false;
}
int main(int argc, char** argv) {
/*  ������ ��������� �������*/
    double L=200; //����
    double a0=85600; //���/�
    double T0=0.1; //�
    double q=0.98; //� ����� �������

    // ��� ������ 9
    double Topt=T0/2; //�������� �� �������� (�� ���� i �� ���� j), � ������� �� ���������, ���������� ���������� ����������� �� �����
    double dc=10000; //��� ��������� ���������� ����������� ����� �����
    int iOminPrevious; //������� ������������ �������� � ������� MO. ����� ��� ��������� ����������� ��������� MB0, ����� �� ���� ��� ��������� ��� ������������ ��� ��� ������ ������������� �������.
    int jOminPrevious;

    double OminCurrent;
    double OminPrevious;
    int iOmin, jOmin; //������ ������ MO �����, � ������� ����������� �������� MO �� ����

    //��� ����������� ������� (����� �������� �� ��������� �������� �����, � �� ������ ���� ������������ ��� ������� ���� ���������� ������������)
    double ToptCurrent; //���������� ��������� �������� �� ��� ���, ���� �� ���� ��������� �������� ������ �� ������ Topt.
    double dTopt=0.001; //� ;��� ���������� ��������.

    setlocale (LC_ALL,"");
/*  ��������� ������ ��� matrix � MPath. ���� matrix - ������� ���������� �� �����*/
    ifstream file("matrix.txt");
    int numberOfVert;
    file >> numberOfVert;
    cout << numberOfVert << endl;

    //������� ��������� � ������ ����� �����(101 - ����� ���, 0 ����� � ����)
    double **matrix = (double**)malloc(sizeof(double)*numberOfVert);
    for(int i = 0; i < numberOfVert; i++) {
        matrix[i] = (double *) malloc(sizeof(double) * numberOfVert);
    }
    //��������� ������� ����� �����
    for(int i = 0; i < numberOfVert; i++) {
        for(int j = 0; j < numberOfVert; j++) { file >> matrix[i][j];
        }
    }
    file.close();
    cout << "Distance matrix:" << endl;
    printMatrix(matrix, numberOfVert);
    //��������� ������ ��� ������� ���������
    int **MPath=(int **)malloc(sizeof(int*)*numberOfVert); //������� ������������� �� ����� ��� ������, �� ����� ��� ���������
    for(int i=0; i<numberOfVert; i++)
    {
        MPath[i]=(int*)malloc(sizeof(int)*numberOfVert);
    }

/*  ��������� � matrix ������� ���������� �����. � � MPath ������� ��������� ������������ */
    originalFloydWarshall(matrix, numberOfVert, MPath);
    cout << "Shortest distance matrix(Floyd):" << endl;
    printMatrix(matrix, numberOfVert);
    printMatrixToFile((char *)"matrixFloyd.txt", matrix, numberOfVert);
    cout << "Route matrix(Floyd):" <<endl;
    printMatrix(MPath, numberOfVert, 1);
    printMatrixToFile((char *)"MPath.txt", MPath, numberOfVert, 1);

/*  ���� ������� ������������� ������� � ������������ �����*/
    double **MNodeToNodeLoad=(double**)malloc(sizeof(double*)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MNodeToNodeLoad[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    ifstream file1("MNodeToNodeLoad.txt");
    for(int i=0; i<numberOfVert; i++)
    {
        for(int j=0; j<numberOfVert; j++)
        {
            file1>>MNodeToNodeLoad[i][j];
        }
    }
    cout << "MNodeToNodeLoad:" <<endl;
    printMatrix(MNodeToNodeLoad, numberOfVert); //��������
/*  ������ ������� ������������� �������� �� ����� �����*/
    double **MEdgeLoad=(double**)malloc(sizeof(double*)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MEdgeLoad[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    for(int i=0; i<numberOfVert; i++)
    {
        for(int j=0; j<numberOfVert; j++)
        {
            MEdgeLoad[i][j]=0;
        }
    }
    cout << "MEdgeLoad:" <<endl;
    printMatrix(MEdgeLoad,numberOfVert);
    EdgeLoad(MNodeToNodeLoad, MPath, numberOfVert, MEdgeLoad);
    cout << "MEdgeLoad:" <<endl;
    printMatrix(MEdgeLoad,numberOfVert);
    printMatrixToFile((char *)"MEdgeLoad.txt", MEdgeLoad,numberOfVert);
/*  ������� �������*/
    int **MThreatNumber=(int**)malloc(sizeof(int*)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MThreatNumber[i]=(int*)malloc(sizeof(int)*numberOfVert);
    }
    ThreatNumber(MEdgeLoad, numberOfVert, 1-q , MThreatNumber);
    cout << "MThreatNumber:" <<endl;
    printMatrix(MThreatNumber, numberOfVert);
    printMatrixToFile((char *)"MThreatNumber.txt", MThreatNumber, numberOfVert);
 //   ThreatNumberBruteForce(MEdgeLoad, numberOfVert, 0.02, MThreatNumber);
 //   cout << "MThreatNumberBruteForce:" <<endl;
//    printMatrix(MThreatNumber, numberOfVert);

/*  7 �����. ������� ������������� ������� �� ��� ����� �����*/
    double **MTrafficIntensity=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MTrafficIntensity[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    TrafficIntensity(MThreatNumber, numberOfVert, a0, MTrafficIntensity);
    cout << "MTrafficIntensity:" <<endl;
    printMatrix(MTrafficIntensity, numberOfVert);
    printMatrixToFile((char *)"MTrafficIntensity.txt", MTrafficIntensity, numberOfVert);
/*  8 �����. ������� ���������� ����������� ����� �����*/
    double **MCommunLineCapacity=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MCommunLineCapacity[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    CommunLineCapacity(MTrafficIntensity, numberOfVert, L*8, T0, MCommunLineCapacity);
    cout << "MCommunLineCapacity:" <<endl;
    printMatrix(MCommunLineCapacity, numberOfVert);
    printMatrixToFile((char *)"MCommunLineCapacity.txt", MCommunLineCapacity, numberOfVert);

/*  9 �����. ����������� ���������� ����������� ����� �����===================================================================================*/
/*  ��������� ������*/
    //MB0 �������, � ������� ����� ����������� ���������� �����������. ��������� ������.
    double **MB0=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MB0[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    //MDel ������� �������� Tij �� ������ �����
    double **MDel=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MDel[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    //MDl ������� �������� dlij ����� ������ �� ���������.
    double **MDl=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MDl[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    //MO ������� �������� ������� �������
    double **MO=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MO[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }




    ToptCurrent=Topt+dTopt; //��� ��������� ��������� � �����.


    do{ //���� ��� ���� ����� �������� � MDl �� ��������� Topt
    ToptCurrent-=dTopt;
/*  ������������� MB0*/
    for(int i=0;i< numberOfVert; i++)
    {
        for(int j=0; j<numberOfVert; j++)
        {
            MB0[i][j]=MCommunLineCapacity[i][j];
        }
    }
    cout<<"MB0 init:"<<endl;
    printMatrix(MB0, numberOfVert);


/*  ���������� MO*/
    OCalc(MB0, MTrafficIntensity, MPath, numberOfVert, L, ToptCurrent, dc, MO);
    cout<<"MO firstcalc:"<<endl;
    printMatrix(MO, numberOfVert);

    //���� OminCurrent;
    OminCurrentCalc(MO, numberOfVert, &iOmin, &jOmin, &OminCurrent);

    int counter=0; //������ ������� ��� ��������
    do{
        OminPrevious=OminCurrent;
        iOminPrevious=iOmin; //����� ������ ��� ��������� ������������� ������ ��� ������.
        jOminPrevious=jOmin;
        MB0[iOmin][jOmin]+=dc;
     //   cout<<"iOmin=" <<iOmin <<"jOmin="<< jOmin<<endl;
        /*  ���������� MO*/
        OCalc(MB0, MTrafficIntensity, MPath, numberOfVert, L, ToptCurrent, dc, MO);

        //���� OminCurrent;
        OminCurrentCalc(MO, numberOfVert, &iOmin, &jOmin, &OminCurrent);

  //      cout<<"MO"<<  counter <<":"<< " OminPrevious="<< OminPrevious<<" OminCurrent=" << OminCurrent<<endl;
    //  printMatrix(MO, numberOfVert);
        counter++;

    }while(OminCurrent<OminPrevious);

    //��� ��� ����� �������� � MDl �������, ��������������� ������������ �������� MO. ����� �������� ������������� ������� ��������� MB0, ������� ������� �������� �� ���� �������.
                //��������� MDel
            for(int i=0; i< numberOfVert; i++)
            {
                for(int j=0; j<numberOfVert; j++)
                {
                    if(MB0[i][j]>0) //���� ����� ������������ � ���������
                    {
                   //     if(i==iO && j==jO) //����� ��� �� ����� ������ �������. MB0 �������� ��� ����������� ���������� �����������, � �� ������ ������� �� ��� ��������.
                   //     {
                   //         MDel[i][j]=L*8/(MB0[i][j]+dc-MTrafficIntensity[i][j]);
                   //     }
                   //     else
                      //  {
                            MDel[i][j]=L*8/(MB0[i][j]-MTrafficIntensity[i][j]); //��� ����� ������� ��� �����
                      //  }
                    }
                    else
                    {
                        MDel[i][j]=0;
                    }
                }
            }
     //       cout<<"MDel:"<<endl;
       //     printMatrix(MDel, numberOfVert);
            //��������� MDl
            for(int i=0; i<numberOfVert; i++)
            {
                for(int j=0; j<numberOfVert; j++) //��������� �������� ��� ������� ��������
                {
                    int k1=i;
                    int k2=MPath[i][j];
                    MDl[i][j]=0;        //�������������
                    MDl[i][j]+=MDel[k1][k2];
                    while(k2!=j)
                    {
                        k1=k2;
                        k2=MPath[k2][j];
                        MDl[i][j]+=MDel[k1][k2];
                    }
                }
            }

       //     cout<<"MDl:"<<endl;
       //     printMatrix(MDl, numberOfVert);
        //    system("pause"); //��� �������

    }while(IsGreaterThen(MDl, numberOfVert, Topt));

/*  ����� ����������� 9 ������*/
    cout<<"MB0:"<<endl;
    printMatrix(MB0, numberOfVert);
    printMatrixToFile((char *)"MB0.txt", MB0, numberOfVert);

    cout<<"MDel:"<<endl;
    printMatrix(MDel, numberOfVert);
    printMatrixToFile((char *)"MDel.txt", MDel, numberOfVert);

    cout<<"MDl:"<<endl;
    printMatrix(MDl, numberOfVert);
    printMatrixToFile((char *)"MDl.txt", MDl, numberOfVert);

    /*  ���������� MO � ����������� ��������� �� ���� ����������� ��� ������*/
    MB0[iOminPrevious][jOminPrevious]-=dc; //�������, ��������������� ����������� ���������.
    OCalc(MB0, MTrafficIntensity, MPath, numberOfVert, L, Topt, dc, MO);

    cout<<"MOPrevious (have minimum value which have achived):"<<endl;
    printMatrix(MO, numberOfVert);
    printMatrixToFile((char *)"MOPrevious.txt", MO, numberOfVert); //����� �������, � ������� ���������� ���������� ����������� �������� ������� �������.

    //���� OminCurrent;
    OminCurrentCalc(MO, numberOfVert, &iOmin, &jOmin, &OminCurrent);

    cout<<"Omin="<<OminCurrent<<" In cell:"<< iOmin<< " "<<jOmin <<endl;
    ofstream ofile("MOPrevious.txt", ios_base::app);
    ofile<<"Omin="<<OminCurrent<<" In cell:"<< iOmin<< " "<<jOmin <<endl; //����� ����������� �������� ������� � ������ ������, � ������� ��� �����������.
    cout<<"ToptCurrent="<<ToptCurrent<<endl;
    ofile<<"ToptCurrent="<<ToptCurrent<<endl;
    ofile.close();

/*  ������������ ������*/
    for(int i=0;i<numberOfVert;i++)
    {
        free(matrix[i]);
        free(MPath[i]);
        free(MNodeToNodeLoad[i]);
        free(MEdgeLoad[i]);
        free(MThreatNumber[i]);
        free(MTrafficIntensity[i]);
        free(MCommunLineCapacity[i]);
        //��� 9 ������
        free(MB0[i]);
        free(MDel[i]);
        free(MDl[i]);
        free(MO[i]);
    }
    free(matrix);
    free(MPath);
    free(MNodeToNodeLoad);
    free(MEdgeLoad);
    free(MThreatNumber);
    free(MTrafficIntensity);
    free(MCommunLineCapacity);
    //��� 9 ������
    free(MB0);
    free(MDl);
    free(MDel);
    free(MO);
    return 0;
}
