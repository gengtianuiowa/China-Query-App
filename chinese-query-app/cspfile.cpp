#include "cspfile.h"
#include"math.h"

CSpFile::CSpFile()
{
    this->datState=false;
    this->optState=false;
    this->txtState=false;
}

void CSpFile::ShowProperties()
{
    cout<<"There are "<<this->PropertyCapacity<<" properties stored."<<endl;
    for (int Counter=0;Counter<this->PropertyCapacity;Counter++)
        this->Properties[Counter].ShowProperty();
}

void CSpFile::ShowCities()
{
    cout<<"There are "<<this->CityCapacity<<" cities in total."<<endl;
    for (int Counter=0;Counter<this->CityCapacity;Counter++)
        this->Cities[Counter].ShowCity();
}

bool CSpFile::LoadFile()
{
//    if (this->LoadDat());
//        //cout<<"DAT file loaded."<<endl;
//    else
//    {
//        //cout<<"DAT file loading failed."<<endl;
//        return false;
//    }
//    if (this->LoadOpt());
//        //cout<<"OPT file loaded."<<endl;
//    else
//    {
//        //cout<<"OPT file loading failed."<<endl;
//        return false;
//    }
//    if (this->LoadTxt());
//        //cout<<"TXT file loaded."<<endl;
//    else
//    {
//        //cout<<"TXT file loading failed."<<endl;
//        return false;
//    }
    if(this->LoadDat()&&this->LoadOpt()&&this->LoadTxt())
    return true;
    else
        return false;
}

bool CSpFile::LoadDat()
{
    //DAT FILE
    fstream DATSource(this->DAT,ios::in);
    if (!DATSource)
    {
        //cout<<"DAT file doesn't exist!"<<endl;
        this->datState=false;
        return false;
    }
    //else
        //cout<<"DAT file found."<<endl<<"Loading DAT file..."<<endl;
    istreambuf_iterator<char>beg(DATSource),end;
    string RawDataDAT(beg,end);
    int Length=RawDataDAT.length();
    int Compass=0;
    int Scout=0;
    int LineCounter=0;
    bool LayerData=false;
    bool ObjectData=false;
    int LayerCounter=0;
    int ObjectCounter=0;
    int PointCounter=0;
    for (;Compass<Length;)
    {
        int LineCapacity=0;
        while (RawDataDAT[Scout]!='\n'&&Scout<Length)
        {
            Scout++;
            LineCapacity++;
        }
        char *TempLine=new char[LineCapacity+1];
        Scout=Compass;
        for (;Scout<Compass+LineCapacity+1;Scout++)
            if (Scout<Length)
                TempLine[Scout-Compass]=RawDataDAT[Scout];
            else
                TempLine[Scout-Compass]='\n';
        string TempLineTrans(TempLine);
        const char* TempData=TempLineTrans.c_str();
        LineCounter++;
        if (!LayerData)
        {
            switch (LineCounter)
            {
            case 1:
                sscanf(TempData,"%d,%d",&this->Database.Left,&this->Database.Top);break;
            case 2:
                sscanf(TempData,"%d,%d",&this->Database.Right,&this->Database.Bottom);break;
            case 3:
                sscanf(TempData,"%d",&this->Database.LayerCapacity);
                this->Database.GEOLayer=new CSpLayer[this->Database.LayerCapacity];
                LayerData=true;
                LineCounter=0;
            }
        }
        else if (!ObjectData)
        {
            switch (LineCounter)
            {
            case 1:
                sscanf(TempData,"%d",&this->Database.GEOLayer[LayerCounter].NameCapacity);break;
            case 2:
            {
                char* TempName=new char[this->Database.GEOLayer[LayerCounter].NameCapacity];
                sscanf(TempData,"%[^\n]",TempName);
                this->Database.GEOLayer[LayerCounter].LayerName=TempName;
                break;
            }
            case 3:
                sscanf(TempData,"%d",&this->Database.GEOLayer[LayerCounter].ObjectCapacity);
                this->Database.GEOLayer[LayerCounter].GEOObject=new CSpObject[this->Database.GEOLayer[LayerCounter].ObjectCapacity];
                ObjectData=true;
                LineCounter=0;
            }
        }
        else
        {
            if (LineCounter==1)
            {
                sscanf(TempData,"%d",&this->Database.GEOLayer[LayerCounter].GEOObject[ObjectCounter].Dimension);
                int TempPointCounter=0;
                int PointDrone=Scout;
                int TempCompass=Scout;
                CSpPoint TEMPP;
                TEMPP.X=0;
                TEMPP.Y=0;
                do
                {
                    int PLineCapacity=0;
                    while (RawDataDAT[PointDrone]!='\n'&&PointDrone<Length)
                    {
                        PointDrone++;
                        PLineCapacity++;
                    }
                    char *TempPLine=new char[PLineCapacity+1];
                    PointDrone=TempCompass;
                    for (;PointDrone<TempCompass+PLineCapacity+1;PointDrone++)
                        if (PointDrone<Length)
                            TempPLine[PointDrone-TempCompass]=RawDataDAT[PointDrone];
                        else
                            TempPLine[PointDrone-TempCompass]='\n';
                    string TempPLineTrans(TempPLine);
                    const char* TempPData=TempPLineTrans.c_str();
                    sscanf(TempPData,"%d,%d",&TEMPP.X,&TEMPP.Y);
                    if (!TEMPP.IsEnd())
                    {
                        TempPointCounter++;
                        TempCompass=TempCompass+PLineCapacity+1;
                    }
                }while(!TEMPP.IsEnd());
                this->Database.GEOLayer[LayerCounter].GEOObject[ObjectCounter].PointCapacity=TempPointCounter;
                this->Database.GEOLayer[LayerCounter].GEOObject[ObjectCounter].GEOPoint=new CSpPoint[this->Database.GEOLayer[LayerCounter].GEOObject[ObjectCounter].PointCapacity];
            }
            else
            {
                CSpPoint IFEND;
                sscanf(TempData,"%d,%d",&IFEND.X,&IFEND.Y);
                if (!IFEND.IsEnd())
                {
                    this->Database.GEOLayer[LayerCounter].GEOObject[ObjectCounter].GEOPoint[PointCounter]=IFEND;
                    PointCounter++;
                }
                else
                {
                    PointCounter=0;
                    LineCounter=0;
                    ObjectCounter++;
                    if (ObjectCounter==this->Database.GEOLayer[LayerCounter].ObjectCapacity)
                    {
                        ObjectCounter=0;
                        ObjectData=false;
                        LayerCounter++;
                        if (LayerCounter==this->Database.LayerCapacity)
                            LayerData=false;
                    }
                }
            }
        }
        free(TempLine);
        Compass=Compass+LineCapacity+1;
    }
    RawDataDAT.erase(0,Length); 
    this->datState=true;
    return true;
}

bool CSpFile::LoadOpt()
{
    //OPT FILE
    fstream OPTSource(this->OPT,ios::in);
    if (!OPTSource)
    {
        //cout<<"OPT file doesn't exist!"<<endl;
        this->optState=false;
        return false;
    }
//    else
//        cout<<"OPT file found."<<endl<<"Loading OPT file..."<<endl;
    istreambuf_iterator<char>beg(OPTSource),end;
    string RawDataOPT(beg,end);
    int Length=RawDataOPT.length();
    int Compass=0;
    int Scout=0;
    int LineCounter=0;
    char DataSwitch='C';
    int LayerCounter=0;
    for (;Compass<Length;)
    {
        int LineCapacity=0;
        while (RawDataOPT[Scout]!='\n'&&Scout<Length)
        {
            Scout++;
            LineCapacity++;
        }
        char *TempLine=new char[LineCapacity+1];
        Scout=Compass;
        for (;Scout<Compass+LineCapacity+1;Scout++)
            if (Scout<Length)
                TempLine[Scout-Compass]=RawDataOPT[Scout];
            else
                TempLine[Scout-Compass]='\n';
        string TempLineTrans(TempLine);
        const char* TempData=TempLineTrans.c_str();
        LineCounter++;
        if (DataSwitch=='C')
        {
            sscanf(TempData," %d",&this->PropertyCapacity);
            this->Properties=new CSpProperty[this->PropertyCapacity];
            DataSwitch='L';
            LineCounter=0;
        }
        else if (DataSwitch=='L')
        {
            switch (LineCounter)
            {
            case 1:
            {
                char* TempName=new char[LineCapacity];
                sscanf(TempData,"%[^\n]",TempName);
                this->Properties[LayerCounter].LayerType=TempName;
                break;
            }
            case 2:
            {
                sscanf(TempData," %d",&this->Properties[LayerCounter].IsLinear);
                if (!this->Properties[LayerCounter].IsLinear)
                {
                    this->Properties[LayerCounter].Width=0;
                    this->Properties[LayerCounter].LineColor.Red=0;
                    this->Properties[LayerCounter].LineColor.Blue=0;
                    this->Properties[LayerCounter].LineColor.Green=0;
                    LineCounter=LineCounter+2;
                }
                break;
            }
            case 3:
                sscanf(TempData,"%f",&this->Properties[LayerCounter].Width);break;
            case 4:
                sscanf(TempData,"%d, %d, %d",&this->Properties[LayerCounter].LineColor.Red,&this->Properties[LayerCounter].LineColor.Green,&this->Properties[LayerCounter].LineColor.Blue);break;
            case 5:
            {
                sscanf(TempData,"%d",&this->Properties[LayerCounter].IsPlanar);
                if (!this->Properties[LayerCounter].IsPlanar)
                {
                    this->Properties[LayerCounter].PlaneColor.Red=0;
                    this->Properties[LayerCounter].PlaneColor.Blue=0;
                    this->Properties[LayerCounter].PlaneColor.Green=0;
                    LayerCounter++;
                    LineCounter=0;
                }
                break;
            }
            case 6:
                sscanf(TempData,"%d, %d, %d",&this->Properties[LayerCounter].PlaneColor.Red,&this->Properties[LayerCounter].PlaneColor.Green,&this->Properties[LayerCounter].PlaneColor.Blue);
                LineCounter=0;
                LayerCounter++;
            }
        }
        if (LayerCounter==this->PropertyCapacity)
        {
            LayerCounter=0;
            DataSwitch='C';
        }
        free(TempLine);
        Compass=Compass+LineCapacity+1;
    }
    RawDataOPT.erase(0,Length);
    this->optState=true;
    return true;
}

bool CSpFile::LoadTxt()
{
    //TXT FILE
    fstream TXTSource(this->TXT,ios::in);
    if (!TXTSource)
    {
        //cout<<"TXT file doesn't exist!"<<endl;
        this->txtState=false;
        return false;
    }
//    else
//        cout<<"TXT file found."<<endl<<"Loading TXT file..."<<endl;
    istreambuf_iterator<char>beg(TXTSource),end;
    string RawDataTXT(beg,end);
    int Length=RawDataTXT.length();
    int Compass=0;
    int Scout=0;
    int LineCounter=0;
    int CityCounter=0;
    while (Scout<Length)
    {
        if (RawDataTXT[Scout]=='\n')
            LineCounter++;
        Scout++;
    }
    Scout=0;
    LineCounter++;
    this->CityCapacity=LineCounter;
    this->Cities=new CSpCities[this->CityCapacity];
    for (;Compass<Length;)
    {
        int LineCapacity=0;
        int NameCapacity=0;
        while (RawDataTXT[Scout]!='\n'&&Scout<Length)
        {
            if (RawDataTXT[Scout]!=' ')
                NameCapacity++;
            Scout++;
            LineCapacity++;
        }
        char *TempLine=new char[LineCapacity+1];
        Scout=Compass;
        for (;Scout<Compass+LineCapacity+1;Scout++)
            if (Scout<Length)
                TempLine[Scout-Compass]=RawDataTXT[Scout];
            else
                TempLine[Scout-Compass]='\n';
        string TempLineTrans(TempLine);
        const char* TempData=TempLineTrans.c_str();
        char* TempName=new char[NameCapacity];
        sscanf(TempData,"%[^ ] %d %d",TempName,&this->Cities[CityCounter].Position.X,&this->Cities[CityCounter].Position.Y);
        this->Cities[CityCounter].CityName=TempName;
        CityCounter++;
        free(TempLine);
        Compass=Compass+LineCapacity+1;
    }
    RawDataTXT.erase(0,Length);
    this->txtState=true;
    return true;
}

void CSpFile::createIndex()
{
    int rowlength=(this->Database.Right-this->Database.Left)/4;
    int collength=(this->Database.Top-this->Database.Bottom)/3;
    int x;int y;
    for(int i=0;i<CityCapacity;i++)
    {
        x=this->Cities[i].Position.X;
        y=this->Cities[i].Position.Y;
    if(this->Database.Bottom<=y&&y<this->Database.Bottom+collength)//网格索引，先判断行，再判断列
    {
        if(this->Database.Left<=x&&x<this->Database.Left+rowlength)
            this->index.index1.push_back(this->Cities[i]);
        if(this->Database.Left+rowlength<=x&&x<this->Database.Left+2*rowlength)
            this->index.index2.push_back(this->Cities[i]);
        if(this->Database.Left+2*rowlength<=x&&x<this->Database.Left+3*rowlength)
            this->index.index3.push_back(this->Cities[i]);
        if(this->Database.Left+3*rowlength<=x&&x<=this->Database.Right)
            this->index.index4.push_back(this->Cities[i]);
     }
    if(this->Database.Bottom+collength<=y&&y<this->Database.Bottom+2*collength)//网格索引，先判断行，再判断列
    {
        if(this->Database.Left<=x&&x<this->Database.Left+rowlength)
            this->index.index5.push_back(this->Cities[i]);
        if(this->Database.Left+rowlength<=x&&x<this->Database.Left+2*rowlength)
            this->index.index6.push_back(this->Cities[i]);
        if(this->Database.Left+2*rowlength<=x&&x<this->Database.Left+3*rowlength)
            this->index.index7.push_back(this->Cities[i]);
        if(this->Database.Left+3*rowlength<=x&&x<=this->Database.Right)
            this->index.index8.push_back(this->Cities[i]);
     }
    if(this->Database.Bottom+2*collength<=y&&y<this->Database.Top)//网格索引，先判断行，再判断列
    {
        if(this->Database.Left<=x&&x<this->Database.Left+rowlength)
            this->index.index9.push_back(this->Cities[i]);
        if(this->Database.Left+rowlength<=x&&x<this->Database.Left+2*rowlength)
            this->index.index10.push_back(this->Cities[i]);
        if(this->Database.Left+2*rowlength<=x&&x<this->Database.Left+3*rowlength)
            this->index.index11.push_back(this->Cities[i]);
        if(this->Database.Left+3*rowlength<=x&&x<=this->Database.Right)
            this->index.index12.push_back(this->Cities[i]);
     }
    }
}

bool CSpFile::findInindex(int x, int y)
{
    int rowlength=(this->Database.Right-this->Database.Left)/4;
    int collength=(this->Database.Top-this->Database.Bottom)/3;
        if(x<this->Database.Left||x>this->Database.Right||y<this->Database.Bottom||y>this->Database.Top)
            return false;
        else
        {
        if(this->Database.Bottom<=y&&y<this->Database.Bottom+collength)//网格索引，先判断行，再判断列
    {
        if(this->Database.Left<=x&&x<this->Database.Left+rowlength)
            for (list<CSpCities>::iterator it=this->index.index1.begin();it!=this->index.index1.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
        if(this->Database.Left+rowlength<=x&&x<this->Database.Left+2*rowlength)
            for (list<CSpCities>::iterator it=this->index.index2.begin();it!=this->index.index2.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
        if(this->Database.Left+2*rowlength<=x&&x<this->Database.Left+3*rowlength)
            for (list<CSpCities>::iterator it=this->index.index3.begin();it!=this->index.index3.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
        if(this->Database.Left+3*rowlength<=x&&x<=this->Database.Right)
            for (list<CSpCities>::iterator it=this->index.index4.begin();it!=this->index.index4.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
     }
        if(this->Database.Bottom+collength<=y&&y<this->Database.Bottom+2*collength)//网格索引，先判断行，再判断列
    {
        if(this->Database.Left<=x&&x<this->Database.Left+rowlength)
            for (list<CSpCities>::iterator it=index.index5.begin();it!=this->index.index5.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
        if(this->Database.Left+rowlength<=x&&x<this->Database.Left+2*rowlength)
            for (list<CSpCities>::iterator it=index.index6.begin();it!=this->index.index6.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
        if(this->Database.Left+2*rowlength<=x&&x<this->Database.Left+3*rowlength)
            for (list<CSpCities>::iterator it=index.index7.begin();it!=this->index.index7.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
        if(this->Database.Left+3*rowlength<=x&&x<=this->Database.Right)
            for (list<CSpCities>::iterator it=index.index8.begin();it!=this->index.index8.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
     }
        if(this->Database.Bottom+2*collength<=y&&y<this->Database.Top)//网格索引，先判断行，再判断列
    {
        if(this->Database.Left<=x&&x<this->Database.Left+rowlength)
            for (list<CSpCities>::iterator it=index.index9.begin();it!=this->index.index9.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
        if(this->Database.Left+rowlength<=x&&x<this->Database.Left+2*rowlength)
            for (list<CSpCities>::iterator it=index.index10.begin();it!=this->index.index10.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
        if(this->Database.Left+2*rowlength<=x&&x<this->Database.Left+3*rowlength)
            for (list<CSpCities>::iterator it=index.index11.begin();it!=this->index.index11.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
        if(this->Database.Left+3*rowlength<=x&&x<=this->Database.Right)
            for (list<CSpCities>::iterator it=index.index12.begin();it!=this->index.index12.end();++it)
            {
                CSpCities temp=*it;
                if(x==temp.Position.X&&y==temp.Position.Y)
                {
                    this->cityFromindex=temp;
                    return true;
                }
            }
     }
        return false;
        }
}

bool CSpFile::cursorPositiontoPoint(int x, int y)
{
    int rowlength=(this->Database.Right-this->Database.Left)/4;
    int collength=(this->Database.Top-this->Database.Bottom)/3;
    int judgevalue=70000;
        if(x<this->Database.Left||x>this->Database.Right||y<this->Database.Bottom||y>this->Database.Top)
            return false;
        else
        {
        if(this->Database.Bottom<=y&&y<this->Database.Bottom+collength)//网格索引，先判断行，再判断列
    {
        if(this->Database.Left<=x&&x<this->Database.Left+rowlength)
            for (list<CSpCities>::iterator it=this->index.index1.begin();it!=this->index.index1.end();++it)
            {
                CSpCities temp=*it;
                if(abs(x-temp.Position.X)+abs(y-temp.Position.Y)<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
        if(this->Database.Left+rowlength<=x&&x<this->Database.Left+2*rowlength)
            for (list<CSpCities>::iterator it=this->index.index2.begin();it!=this->index.index2.end();++it)
            {
                CSpCities temp=*it;
                if(abs(x-temp.Position.X)+abs(y-temp.Position.Y)<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
        if(this->Database.Left+2*rowlength<=x&&x<this->Database.Left+3*rowlength)
            for (list<CSpCities>::iterator it=this->index.index3.begin();it!=this->index.index3.end();++it)
            {
                CSpCities temp=*it;
                if(abs(x-temp.Position.X)+abs(y-temp.Position.Y)<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
        if(this->Database.Left+3*rowlength<=x&&x<=this->Database.Right)
            for (list<CSpCities>::iterator it=this->index.index4.begin();it!=this->index.index4.end();++it)
            {
                CSpCities temp=*it;
                if(abs(x-temp.Position.X)+abs(y-temp.Position.Y)<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
     }
        if(this->Database.Bottom+collength<=y&&y<this->Database.Bottom+2*collength)//网格索引，先判断行，再判断列
    {
        if(this->Database.Left<=x&&x<this->Database.Left+rowlength)
            for (list<CSpCities>::iterator it=index.index5.begin();it!=this->index.index5.end();++it)
            {
                CSpCities temp=*it;
                if(abs(x-temp.Position.X)+abs(y-temp.Position.Y)<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
        if(this->Database.Left+rowlength<=x&&x<this->Database.Left+2*rowlength)
            for (list<CSpCities>::iterator it=index.index6.begin();it!=this->index.index6.end();++it)
            {
                CSpCities temp=*it;
                if(abs(x-temp.Position.X)+abs(y-temp.Position.Y)<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
        if(this->Database.Left+2*rowlength<=x&&x<this->Database.Left+3*rowlength)
            for (list<CSpCities>::iterator it=index.index7.begin();it!=this->index.index7.end();++it)
            {
                CSpCities temp=*it;
                if((abs(x-temp.Position.X)+abs(y-temp.Position.Y))<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
        if(this->Database.Left+3*rowlength<=x&&x<=this->Database.Right)
            for (list<CSpCities>::iterator it=index.index8.begin();it!=this->index.index8.end();++it)
            {
                CSpCities temp=*it;
                if(abs(x-temp.Position.X)+abs(y-temp.Position.Y)<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
     }
        if(this->Database.Bottom+2*collength<=y&&y<this->Database.Top)//网格索引，先判断行，再判断列
    {
        if(this->Database.Left<=x&&x<this->Database.Left+rowlength)
            for (list<CSpCities>::iterator it=index.index9.begin();it!=this->index.index9.end();++it)
            {
                CSpCities temp=*it;
                if(abs(x-temp.Position.X)+abs(y-temp.Position.Y)<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
        if(this->Database.Left+rowlength<=x&&x<this->Database.Left+2*rowlength)
            for (list<CSpCities>::iterator it=index.index10.begin();it!=this->index.index10.end();++it)
            {
                CSpCities temp=*it;
                if(abs(x-temp.Position.X)+abs(y-temp.Position.Y)<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
        if(this->Database.Left+2*rowlength<=x&&x<this->Database.Left+3*rowlength)
            for (list<CSpCities>::iterator it=index.index11.begin();it!=this->index.index11.end();++it)
            {
                CSpCities temp=*it;
                if(abs(x-temp.Position.X)+abs(y-temp.Position.Y)<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
        if(this->Database.Left+3*rowlength<=x&&x<=this->Database.Right)
            for (list<CSpCities>::iterator it=index.index12.begin();it!=this->index.index12.end();++it)
            {
                CSpCities temp=*it;
                if(abs(x-temp.Position.X)+abs(y-temp.Position.Y)<=judgevalue)//判断是否和某点的距离为3
                {
                    this->findInindex(temp.Position.X,temp.Position.Y);
                    return true;
                }
            }
     }
        return false;
        }
}
