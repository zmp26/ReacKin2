#include "KMassTable.h"

void KMassTable::Init(const char* file)
{
  int n,z,a;
  int counter=0;
  char sym[3];
  float massexcess;
  mFileName=file;
  //std::cout << "initializing mass table from file "<<file<<" ..."<<std::endl;

  std::ifstream input_file(file);
  if (!input_file){
    std::cout << "error opening file"<<std::endl;
    exit(1);
  }

  while (input_file) {
    //    cout << "before read"<<endl;
     input_file>>n;
    if (n >= 0) {
      input_file>>z>>a>>sym>>massexcess;
      mEntries[counter].N=n;
      mEntries[counter].Z=z;
      mEntries[counter].A=a;
      //      mEntries[counter].Symbol=sym;
      //      cout <<"n,z,a,sym:" << n<<" "<<z<<" "<<a<<" "<<sym<<endl;
      //      strcpy(sym,mEntries[counter].Symbol);
      //      cout <<"point 1"<<endl;
      sprintf(mEntries[counter].Symbol,"%s",sym);
      mEntries[counter].MassExcess=massexcess;
      //      cout <<"point 2"<<endl;
      counter++;
      //      cout << "end of loop"<<endl;
    }
  }
  //std::cout << "number of entries= "<<counter<<std::endl;
  mNumberOfEntries = counter;
  
  //std::cout << "mass table read with " << mNumberOfEntries <<" entries."<<std::endl ;
  input_file.close();
  //cstd::cout << "file "<<file<< " closed."<<std::endl;

  this->mInitStatus = true;

};

void KMassTable::Print(void)
{
  for (int i=0; i<mNumberOfEntries; i++)
    {
      PrintEntry(i);
    }
};

void KMassTable::PrintEntry(int i)
{
  std::cout << "(N,Z,A)=("<<mEntries[i].N<<","<<mEntries[i].Z<<","<<
    mEntries[i].A<<") Symbol = "<<mEntries[i].Symbol<<
				   "Mass excess= "<<mEntries[i].MassExcess<<std::endl;
};

int KMassTable::Lookup(int n, int z, int a)
{
  int counter=0;
  int returnvalue=0;
  //  cout << "n,z,a= "<<n<<" "<<z<<" "<<a<<endl;
  if (a==0) a=n+z;
  for (counter=0;counter<mNumberOfEntries;counter++) {
    if (mEntries[counter].N==n && mEntries[counter].Z==z &&
	mEntries[counter].A==a) {
      returnvalue=1;
      return (returnvalue);
    }
  }
  return (returnvalue);
};

int KMassTable::Lookup(char *symbol, int a)
{
  char test[3];
  sprintf(test,"%s",symbol);
  int counter=0;
  int returnvalue=0;
  //  cout << "symbol= "<<symbol<<endl;
  for (counter=0;counter<mNumberOfEntries;counter++) {
    if (!strcmp(mEntries[counter].Symbol,test) && mEntries[counter].A==a){
      returnvalue=1;
      return (returnvalue);
    }
  }
  return (returnvalue);
};

float KMassTable::GetMassExcess(const char *symbol, int a)
{
  char test[3];
  sprintf(test,"%s",symbol);
  int counter=0;
  float returnvalue=0;
  for (counter=0;counter<mNumberOfEntries;counter++) {
    if (!strcmp(mEntries[counter].Symbol,test) && mEntries[counter].A==a){
      returnvalue=mEntries[counter].MassExcess;
      return (returnvalue);
    }
  }
  return (returnvalue);
};

float KMassTable::GetMassExcess(int n=0, int z=0, int a=0)
{
  int counter=0;
  float returnvalue=0;
  if (a==0) a=n+z;
  for (counter=0;counter<mNumberOfEntries;counter++) {
    if (mEntries[counter].N==n && mEntries[counter].Z==z &&
	mEntries[counter].A==a) {
      returnvalue=mEntries[counter].MassExcess;
      return (returnvalue);
    }
  }
  return (returnvalue);
};

const char* KMassTable::GetSymbol(int z=0)
{
  int counter=0;
  const char* returnvalue="n ";
  for (counter=0;counter<mNumberOfEntries;counter++) {
    if (mEntries[counter].Z==z) {
      returnvalue=mEntries[counter].Symbol;
      return (returnvalue);
    }
  }
  return (returnvalue);
};

int KMassTable::GetZ(char *symbol)
{
  char test[3];
  sprintf(test,"%s",symbol);
  int counter=0;
  int returnvalue=0;
  for (counter=0;counter<mNumberOfEntries;counter++) {
    if (!strcmp(mEntries[counter].Symbol,test)){
      returnvalue=mEntries[counter].Z;
      return (returnvalue);
    }
  }
  return (returnvalue);
};

//added by zmp:
bool KMassTable::GetInitStatus(){//return true for initialized, false otherwise
  return this->mInitStatus;
};

float KMassTable::GetMassMeV(const char* symbol, int a){//return the mass in MeV/c^2 instead of mass excess - simpler for the end user! (its me im the end user)
  return (a*AMU + this->GetMassExcess(symbol,a)*0.001);
};

float KMassTable::GetMassMeV(int n=0, int z=0, int a=0){
	int counter=0;
	float retval = 0;
	if(a==0) a=n+z;
	for(counter=0;counter<mNumberOfEntries;counter++){
		if(mEntries[counter].N==n && mEntries[counter].Z==z && mEntries[counter].A==a){
			retval = mEntries[counter].MassExcess*0.001 + a*AMU;
			return retval;
		}
	}


	return retval;
};

int KMassTable::GetN(char *symbol, int a){
  char test[3];
  sprintf(test,"%s",symbol);
  
  for(int counter=0; counter<mNumberOfEntries; counter++){
    if(!strcmp(mEntries[counter].Symbol,test) && mEntries[counter].A == a){
      return mEntries[counter].A - mEntries[counter].Z;
    }
  }

  return -1;
}