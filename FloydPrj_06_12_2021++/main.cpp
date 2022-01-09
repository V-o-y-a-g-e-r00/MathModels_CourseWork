#include <iostream>
#include <algorithm>
#include <fstream>
//#include <iomanip> если не будет работать сброс ширины поля в консоли
//Максимальное значение веса = 100
#include <cmath>

#define INF 101

using namespace std;

template<typename T>
void printMatrix(T** matrix, int numberOfVert, int OrderFlag=0) //int OrderFlag - флаг нумерации значений в матрице. 0 с нуля; 1 - с единицы(будет прибалена 1 к каждому элементу).
{
    double CtrlSum=0; //контрольная сумма
    for(int i = 0; i < numberOfVert; i++) {
        for(int j = 0; j < numberOfVert; j++)
        {
            cout.width(7);
            cout << matrix[i][j]+OrderFlag<< " ";
            CtrlSum+=matrix[i][j]+OrderFlag; //нумерация с 1
        }
        cout << endl;
    }
    cout << "CtrlSum=" << CtrlSum<<endl;
}
template<typename T>
void printMatrixToFile(char* filename, T** matrix, int numberOfVert, int OrderFlag=0) //int OrderFlag - флаг нумерации значений в матрице. 0 с нуля; 1 - с единицы(будет прибалена 1 к каждому элементу).
{
    ofstream ofile(filename);
    if(!filename) cout <<"Cannot open file " << filename << endl;

    double CtrlSum=0; //контрольная сумма
    for(int i = 0; i < numberOfVert; i++) {
        for(int j = 0; j < numberOfVert; j++)
        {
            ofile<<std::fixed;
        //    cout.width(7);
           // cout << matrix[i][j]+OrderFlag<< " ";
            ofile<<matrix[i][j]+OrderFlag<< " ";
            CtrlSum+=matrix[i][j]+OrderFlag; //нумерация с 1
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
   // cout << std::resetiosflags(std::ios::fixed); //если не будет работать сброс ширины поля в консоли
}
//matrix - матрица смежности
void originalFloydWarshall(double **matrix, int numberOfVert, int **MPath) { //double **matrix-матрица исходных расстояний, туда же пишется результат, int numberOfVert-число вершин, int **MPath-матрица маршрутизации
  //  int MPath[numberOfVert][numberOfVert]; //матрица 1-ых элеметов в пути от i до j;  Т.е. путь от i до j такой: i->MPath[i][j]->...->j
    for(int i=0; i< numberOfVert; i++) //заполняем матрицу стартовыми значениями.
    {
        for(int j=0; j< numberOfVert; j++)
        {
            MPath[i][j]=j;
        }
    }
    //Пробегаемся по всем вершинам и ищем более короткий путь
    //через вершину k
    for(int k = 0; k < numberOfVert; k++) {
        for(int i = 0; i < numberOfVert; i++) {
            for(int j = 0; j < numberOfVert; j++) {
                //Новое значение ребра равно минимальному между старым
                //и суммой ребер i <-> k + k <-> j (если через k пройти быстрее)
                if(matrix[i][j]>matrix[i][k]+matrix[k][j])
                {
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                    MPath[i][j]=MPath[i][k];
                }
             //   matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);

            }
        }
    }
//    int CtrlSum=0; //контрольная сумма
  //  cout << "MPath[i][j]:" <<endl;
 //   for(int i=0; i< numberOfVert;i++)
//    {
 //       for(int j=0;j< numberOfVert; j++)
  //      {
  //          cout << MPath[i][j]+1 << " "; //нумерация с 1
   //         CtrlSum+=MPath[i][j]+1; //нумерация с 1
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
/*    int k; //Нужно преобразовывать к long double. long long не прокатит. */
    long double p, numerator, sum;
    for(i=0; i<numberOfVert; i++)
    {
        for(j=0;j<numberOfVert;j++)
        {
            if(MEdgeLoad[i][j]>0)
            {
        /*        MThreatNumber[i][j]=1; //все работает. Короче, тут 2 варианта. Тот, который в комментариях, считает в лоб. Его результаты отличаются от результатов 2го варианта в пределах единицы. Надо полагать, что 2 вариант точнее, т.к. при его использовании меньше операций с большими числами
                sum=0;
                for(k=0; k<MThreatNumber[i][j]; k++) //ищем знаменатель формулы
                {
                    sum+=pow((long double)MEdgeLoad[i][j], k)/tgamma((long double)k+1); //MEdgeLoad тоже нужно преобразовывать к long double, иначе pow возвращает бесконечность.
                }
                p=pow((long double)MEdgeLoad[i][j], MThreatNumber[i][j])/tgamma((long double)MThreatNumber[i][j]+1)/sum; */
                MThreatNumber[i][j]=1;
                numerator=MEdgeLoad[i][j];
                sum=1+MEdgeLoad[i][j];
                p=numerator/sum;
                while(p>p0)
                {
        /*          MThreatNumber[i][j]++; //все работает
                    sum=0;
                 //   cout<< "sum:"<<endl;
                    for(k=0; k<MThreatNumber[i][j]; k++) //ищем знаменатель формулы
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
    int k; //Нужно преобразыовывать к long double. long long не прокатит.
    long double p, sum;
    for(i=11; i<numberOfVert; i++)
    {
        for(j=11;j<numberOfVert;j++)
        {
            if(MEdgeLoad[i][j]>0)
            {
                MThreatNumber[i][j]=1; //все работает. Короче, тут 2 варианта. Тот, который в комментариях, считает в лоб. Его результаты отличаются от результатов 2го варианта в пределах единицы. Надо полагать, что 2 вариант точнее, т.к. при его использовании меньше операций с большими числами
                sum=0;
                for(k=0; k<MThreatNumber[i][j]; k++) //ищем знаменатель формулы
                {
                    sum+=pow((long double)MEdgeLoad[i][j], k)/tgamma((long double)k+1); //MEdgeLoad тоже нужно преобразовывать к long double, иначе pow возвращает бесконечность.
                }
                p=pow((long double)MEdgeLoad[i][j], MThreatNumber[i][j])/tgamma((long double)MThreatNumber[i][j]+1)/sum;
                while(p>p0)
                {
                MThreatNumber[i][j]++; //все работает
                    sum=0;
                 //   cout<< "sum:"<<endl;
                    for(k=0; k<MThreatNumber[i][j]; k++) //ищем знаменатель формулы
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
/*  7 пункт. Матрица интенсивности трафика ПД для линий связи*/
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
/*  8 пункт. Матрица пропускной способности линий связи*/
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
/* 9 пункт. Матрица MO*/
void OCalc(double **MB0, double **MTrafficIntensity, int **MPath, int numberOfVert, double L, double Topt, double dc, double **MO) //результат в MO. Остальные параметры не меняются.
{
    //MDel матрица задержек Tij на линиях связи
    double **MDel=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MDel[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    //MDl матрица задержек dlij между узлами по маршрутам.
    double **MDl=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MDl[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    for(int iO=0; iO< numberOfVert; iO++)
    {
        for(int jO=0; jO<numberOfVert; jO++) //в цикле ищем каждое значение MO[iO][jO]
        {
            //Вычисляем MDel
            for(int i=0; i< numberOfVert; i++)
            {
                for(int j=0; j<numberOfVert; j++)
                {
                    if(MB0[i][j]>0) //если линия используется в маршрутах
                    {
                        if(i==iO && j==jO)
                        {
                            MDel[i][j]=L*8/(MB0[i][j]+dc-MTrafficIntensity[i][j]);
                        }
                        else
                        {
                            MDel[i][j]=L*8/(MB0[i][j]-MTrafficIntensity[i][j]); //это можно вынести вне цикла
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
            //Вычилсяем MDl
            for(int i=0; i<numberOfVert; i++)
            {
                for(int j=0; j<numberOfVert; j++) //вычисляем задержку для каждого маршрута
                {
                    int k1=i;
                    int k2=MPath[i][j];
                    MDl[i][j]=0;        //инициализация
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
            //Вычисляем MO[iO][jO]
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
/* Ищем OminCurrent*/
void OminCurrentCalc(double **MO, int numberOfVert, int *iOmin, int *jOmin, double *OminCurrent)
{
    *OminCurrent=MO[0][0];
    *iOmin=0;
    *jOmin=0;
    for(int iO=0; iO<numberOfVert; iO++)
    {
        for(int jO=0; jO<numberOfVert; jO++)
        {
            if(MO[iO][jO]< *OminCurrent && MO[iO][jO]>0) //есть некоторый шанс, что MO[iO][jO] станет равно 0;
            {
                *OminCurrent=MO[iO][jO];
                *iOmin=iO;
                *jOmin=jO;
            }
        }
    }
}
bool IsGreaterThen(double **MDl, int numberOfVert, double Topt) // если хотя бы один элемент больше заданного времени, то возвращается true иначе false
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
/*  Данные курсового проекта*/
    double L=200; //байт
    double a0=85600; //бит/с
    double T0=0.1; //с
    double q=0.98; //в долях единицы

    // Для пункта 9
    double Topt=T0/2; //Задержка по маршруту (от узла i до узла j), к которой мы стремимся, увеличивая пропускную способность на линии
    double dc=10000; //Шаг изменения пропускной способности линии связи
    int iOminPrevious; //Индексы минимального значения в матрице MO. Нужны для получения предыдущего состояния MB0, чтобы по нему уже построить все интересующие нас для ответа промежуточные матрицы.
    int jOminPrevious;

    double OminCurrent;
    double OminPrevious;
    int iOmin, jOmin; //хранят индекс MO такой, в котором минимальное значение MO из всех

    //Для оптимизации времени (чтобы задержки не превышали заданное время, а не просто были оптимальными для данного шага пропускных способностей)
    double ToptCurrent; //Постепенно уменьшаем задержку до тех пор, пока по всем маршрутам зареджка станет не больше Topt.
    double dTopt=0.001; //с ;Шаг уменьшения задержки.

    setlocale (LC_ALL,"");
/*  Выделение памяти для matrix и MPath. Ввод matrix - матрицы расстояний из файла*/
    ifstream file("matrix.txt");
    int numberOfVert;
    file >> numberOfVert;
    cout << numberOfVert << endl;

    //Матрица смежности с весами ребер графа(101 - ребра нет, 0 ребро в себя)
    double **matrix = (double**)malloc(sizeof(double)*numberOfVert);
    for(int i = 0; i < numberOfVert; i++) {
        matrix[i] = (double *) malloc(sizeof(double) * numberOfVert);
    }
    //Считываем матрицу весов ребер
    for(int i = 0; i < numberOfVert; i++) {
        for(int j = 0; j < numberOfVert; j++) { file >> matrix[i][j];
        }
    }
    file.close();
    cout << "Distance matrix:" << endl;
    printMatrix(matrix, numberOfVert);
    //выделение памяти под матрицу маршрутов
    int **MPath=(int **)malloc(sizeof(int*)*numberOfVert); //матрица маршрутизации не нужна для Флойда, но нужна для курсовика
    for(int i=0; i<numberOfVert; i++)
    {
        MPath[i]=(int*)malloc(sizeof(int)*numberOfVert);
    }

/*  Получение в matrix матрицы кратчайших путей. и в MPath матрицы маршрутов одновременно */
    originalFloydWarshall(matrix, numberOfVert, MPath);
    cout << "Shortest distance matrix(Floyd):" << endl;
    printMatrix(matrix, numberOfVert);
    printMatrixToFile((char *)"matrixFloyd.txt", matrix, numberOfVert);
    cout << "Route matrix(Floyd):" <<endl;
    printMatrix(MPath, numberOfVert, 1);
    printMatrixToFile((char *)"MPath.txt", MPath, numberOfVert, 1);

/*  Ввод матрицы интенсивности трафика в направлениях связи*/
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
    printMatrix(MNodeToNodeLoad, numberOfVert); //проверка
/*  Расчет матрицы интенсивности нагрузок на линии связи*/
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
/*  Матрица потоков*/
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

/*  7 пункт. Матрица интенсивности трафика ПД для линий связи*/
    double **MTrafficIntensity=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MTrafficIntensity[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    TrafficIntensity(MThreatNumber, numberOfVert, a0, MTrafficIntensity);
    cout << "MTrafficIntensity:" <<endl;
    printMatrix(MTrafficIntensity, numberOfVert);
    printMatrixToFile((char *)"MTrafficIntensity.txt", MTrafficIntensity, numberOfVert);
/*  8 пункт. Матрица пропускной способности линий связи*/
    double **MCommunLineCapacity=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MCommunLineCapacity[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    CommunLineCapacity(MTrafficIntensity, numberOfVert, L*8, T0, MCommunLineCapacity);
    cout << "MCommunLineCapacity:" <<endl;
    printMatrix(MCommunLineCapacity, numberOfVert);
    printMatrixToFile((char *)"MCommunLineCapacity.txt", MCommunLineCapacity, numberOfVert);

/*  9 пункт. Оптимизация пропускной способности линий связи===================================================================================*/
/*  Выделение памяти*/
    //MB0 матрица, в которой будут оптимальные пропускные способности. Выделение памяти.
    double **MB0=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MB0[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    //MDel матрица задержек Tij на линиях связи
    double **MDel=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MDel[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    //MDl матрица задержек dlij между узлами по маршрутам.
    double **MDl=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MDl[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }
    //MO матрица значений целевой функции
    double **MO=(double**)malloc(sizeof(double *)*numberOfVert);
    for(int i=0; i<numberOfVert; i++)
    {
        MO[i]=(double*)malloc(sizeof(double)*numberOfVert);
    }




    ToptCurrent=Topt+dTopt; //для избежания ветвления в цикле.


    do{ //цикл для того чтобы значения в MDl не превышали Topt
    ToptCurrent-=dTopt;
/*  Инициализация MB0*/
    for(int i=0;i< numberOfVert; i++)
    {
        for(int j=0; j<numberOfVert; j++)
        {
            MB0[i][j]=MCommunLineCapacity[i][j];
        }
    }
    cout<<"MB0 init:"<<endl;
    printMatrix(MB0, numberOfVert);


/*  Вычисление MO*/
    OCalc(MB0, MTrafficIntensity, MPath, numberOfVert, L, ToptCurrent, dc, MO);
    cout<<"MO firstcalc:"<<endl;
    printMatrix(MO, numberOfVert);

    //ищем OminCurrent;
    OminCurrentCalc(MO, numberOfVert, &iOmin, &jOmin, &OminCurrent);

    int counter=0; //просто счетчик для проверки
    do{
        OminPrevious=OminCurrent;
        iOminPrevious=iOmin; //нужны только для получения промежуточных матриц для ответа.
        jOminPrevious=jOmin;
        MB0[iOmin][jOmin]+=dc;
     //   cout<<"iOmin=" <<iOmin <<"jOmin="<< jOmin<<endl;
        /*  Вычисление MO*/
        OCalc(MB0, MTrafficIntensity, MPath, numberOfVert, L, ToptCurrent, dc, MO);

        //ищем OminCurrent;
        OminCurrentCalc(MO, numberOfVert, &iOmin, &jOmin, &OminCurrent);

  //      cout<<"MO"<<  counter <<":"<< " OminPrevious="<< OminPrevious<<" OminCurrent=" << OminCurrent<<endl;
    //  printMatrix(MO, numberOfVert);
        counter++;

    }while(OminCurrent<OminPrevious);

    //Тут нам нужно получить в MDl матрицу, соответствующую минимальному значению MO. Этому значению соответствует текущее состояние MB0, поэтому считаем задержки по этой матрице.
                //Вычисляем MDel
            for(int i=0; i< numberOfVert; i++)
            {
                for(int j=0; j<numberOfVert; j++)
                {
                    if(MB0[i][j]>0) //если линия используется в маршрутах
                    {
                   //     if(i==iO && j==jO) //здесь уже не нужно делать добавки. MB0 содержит уже оптимальные пропускные способности, и мы просто находим по ним задержки.
                   //     {
                   //         MDel[i][j]=L*8/(MB0[i][j]+dc-MTrafficIntensity[i][j]);
                   //     }
                   //     else
                      //  {
                            MDel[i][j]=L*8/(MB0[i][j]-MTrafficIntensity[i][j]); //это можно вынести вне цикла
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
            //Вычисляем MDl
            for(int i=0; i<numberOfVert; i++)
            {
                for(int j=0; j<numberOfVert; j++) //вычисляем задержку для каждого маршрута
                {
                    int k1=i;
                    int k2=MPath[i][j];
                    MDl[i][j]=0;        //инициализация
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
        //    system("pause"); //для отладки

    }while(IsGreaterThen(MDl, numberOfVert, Topt));

/*  Вывод результатов 9 пункта*/
    cout<<"MB0:"<<endl;
    printMatrix(MB0, numberOfVert);
    printMatrixToFile((char *)"MB0.txt", MB0, numberOfVert);

    cout<<"MDel:"<<endl;
    printMatrix(MDel, numberOfVert);
    printMatrixToFile((char *)"MDel.txt", MDel, numberOfVert);

    cout<<"MDl:"<<endl;
    printMatrix(MDl, numberOfVert);
    printMatrixToFile((char *)"MDl.txt", MDl, numberOfVert);

    /*  Вычисление MO с минимальным значением из всех достигнутых для ответа*/
    MB0[iOminPrevious][jOminPrevious]-=dc; //Матрица, соответствующая предыдущему состоянию.
    OCalc(MB0, MTrafficIntensity, MPath, numberOfVert, L, Topt, dc, MO);

    cout<<"MOPrevious (have minimum value which have achived):"<<endl;
    printMatrix(MO, numberOfVert);
    printMatrixToFile((char *)"MOPrevious.txt", MO, numberOfVert); //пишем матрицу, в которой содержится наименьшее достигнутое значение целевой функции.

    //ищем OminCurrent;
    OminCurrentCalc(MO, numberOfVert, &iOmin, &jOmin, &OminCurrent);

    cout<<"Omin="<<OminCurrent<<" In cell:"<< iOmin<< " "<<jOmin <<endl;
    ofstream ofile("MOPrevious.txt", ios_base::app);
    ofile<<"Omin="<<OminCurrent<<" In cell:"<< iOmin<< " "<<jOmin <<endl; //пишем минимальное значение функции и индекс ячейки, в которой оно достигается.
    cout<<"ToptCurrent="<<ToptCurrent<<endl;
    ofile<<"ToptCurrent="<<ToptCurrent<<endl;
    ofile.close();

/*  Освобождение памяти*/
    for(int i=0;i<numberOfVert;i++)
    {
        free(matrix[i]);
        free(MPath[i]);
        free(MNodeToNodeLoad[i]);
        free(MEdgeLoad[i]);
        free(MThreatNumber[i]);
        free(MTrafficIntensity[i]);
        free(MCommunLineCapacity[i]);
        //Для 9 пункта
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
    //Для 9 пункта
    free(MB0);
    free(MDl);
    free(MDel);
    free(MO);
    return 0;
}
