#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_LINE_LENGTH 100

typedef struct Hasta{
	int HastaNo;
	char HastaAdi[30];
	int HastaYasi;
	char Cinsiyet;
	int Mahkumlukdurum;
	int EngelliOran;
	char KanamaliHastaDurum[15];
	char HastaKayitSaati[6];
	char MuayeneSaati[6];
	int MuayeneSuresi;
	int Oncelik;
	struct Hasta* Next;
}Hasta;

struct Hasta* ilkHasta = NULL;
struct Hasta* sonHasta = NULL;
struct Hasta *KayitOlanlar;
char muayenesaati[6];
char saat[6];
int muayenesirasi=1;

int IsBuyuk(char* saat1, char* saat2) {
    int saat1_saat, saat1_dakika;
    int saat2_saat, saat2_dakika;
    // Saat deðerlerini ayrýþtýrma
    sscanf(saat1, "%d.%d", &saat1_saat, &saat1_dakika);
    sscanf(saat2, "%d.%d", &saat2_saat, &saat2_dakika);

    if((saat1_saat==saat2_saat && saat1_dakika>=saat2_dakika) || (saat1_saat>saat2_saat))
    	return 1;
    else
    	return 0;
  
}

struct Hasta* SiraliEkle(struct Hasta *YeniHasta) {
    struct Hasta* eklenecek = (struct Hasta*)malloc(sizeof(struct Hasta));
    eklenecek=YeniHasta;
    eklenecek->Next = NULL;

    if (ilkHasta == NULL) {
        return eklenecek;
    }

    if (IsBuyuk(ilkHasta->HastaKayitSaati,eklenecek->HastaKayitSaati)) {
        eklenecek->Next = ilkHasta;
        return eklenecek;
    }

    struct Hasta* iter = (struct Hasta*)malloc(sizeof(struct Hasta));
    iter=ilkHasta;
    while (iter->Next != NULL && IsBuyuk(eklenecek->HastaKayitSaati,iter->Next->HastaKayitSaati)) {
        iter = iter->Next;
    }

    eklenecek->Next = iter->Next;
    iter->Next = eklenecek;
    return ilkHasta;
}

void Hastalariyukle() {
    FILE* dosya;
    char satir[MAX_LINE_LENGTH];

    if ((dosya = fopen("hasta.txt", "r")) == NULL) {
        printf("Dosya acilamadi!\n");
        exit(1);
    }


    while (fgets(satir, MAX_LINE_LENGTH, dosya)) {
        struct Hasta* yeniHasta = (struct Hasta*)malloc(sizeof(struct Hasta));

        // Verileri oku ve Hasta yapýsýna atama yap
        char* token;
        token = strtok(satir, ",");
        yeniHasta->HastaNo = atoi(token);
        token = strtok(NULL, ",");
        strcpy(yeniHasta->HastaAdi, token);
        token = strtok(NULL, ",");
        yeniHasta->HastaYasi = atoi(token);
        token = strtok(NULL, ",");
        yeniHasta->Cinsiyet = token[0];
        token = strtok(NULL, ",");
        yeniHasta->Mahkumlukdurum = (strcmp(token, "true ") == 0) ? 1 : 0;
        token = strtok(NULL, ",");
        yeniHasta->EngelliOran = atoi(token);
        token = strtok(NULL, ",");
        strcpy(yeniHasta->KanamaliHastaDurum, token);
        token = strtok(NULL, ",");
        strcpy(yeniHasta->HastaKayitSaati, token);
        //printf("%.2lf\n", atof(token));
        token = strtok(NULL, ",");
        strcpy(yeniHasta->MuayeneSaati, token);
        token = strtok(NULL, ",");
        yeniHasta->MuayeneSuresi = atoi(token);
        token = strtok(NULL, ",");
        yeniHasta->Oncelik = atoi(token);
        yeniHasta->Oncelik=OncelikPuaniHesapla(yeniHasta->HastaYasi,yeniHasta->Mahkumlukdurum,yeniHasta->EngelliOran,yeniHasta->KanamaliHastaDurum);
		yeniHasta->MuayeneSuresi=MuayeneSuresiHesapla(yeniHasta->HastaYasi,yeniHasta->EngelliOran,yeniHasta->KanamaliHastaDurum);
        yeniHasta->Next = NULL;
        ilkHasta=SiraliEkle(yeniHasta);
    }

    fclose(dosya);
}

void HastaNoEkle(){
	struct Hasta *iter=(struct Hasta*)malloc(sizeof(struct Hasta));
	iter=ilkHasta;
	int n=1;
	while(iter!=NULL){
		iter->HastaNo=n;
		iter=iter->Next;
		n++;	
	}
	
}

void Yazdir(struct Hasta *r){
	
	printf("HastaNo\tHastaAdi\tHastaY.\tC  MahkumD.  EngellilikO.  KanamaD.\tKayitS.\tM.Saati M.Sure OncelikP.\n");
	 // Hastalarý ekrana yazdýr
    struct Hasta* gecerliHasta = r;
    while (gecerliHasta != NULL) {
        printf("%d\t%s\t%d\t%c\t%d\t%d\t   %s\t%s\t%.2lf\t%d\t%d\n",
            gecerliHasta->HastaNo, gecerliHasta->HastaAdi, gecerliHasta->HastaYasi,
            gecerliHasta->Cinsiyet, gecerliHasta->Mahkumlukdurum, gecerliHasta->EngelliOran,
           	gecerliHasta->KanamaliHastaDurum, gecerliHasta->HastaKayitSaati, gecerliHasta->MuayeneSaati,
            gecerliHasta->MuayeneSuresi, gecerliHasta->Oncelik);
        	gecerliHasta = gecerliHasta->Next;
    }

    // Bellekten hastalarý temizle
    gecerliHasta = ilkHasta;
    while (gecerliHasta != NULL) {
        struct Hasta* temp = gecerliHasta;
        gecerliHasta = gecerliHasta->Next;
        free(temp);
    }
}

int yaspuani(int yas){
	if(yas<5)
		return 20;
	else if(yas<45)
		return 0;
	else if(yas<65)
		return 15;
	else
	 return 25;
}

int Ismahkum(int mahkumdurumu){
	if (mahkumdurumu)
		return 50;
	else
		return 0;
}

int KanamaDegeri(char KanamaDurumu[15]){
	if(KanamaDurumu=="agirKanama")
		return 50;
	else if(KanamaDurumu=="kanama    ")
		return 20;
	else
		return 0;
}

int OncelikPuaniHesapla(int yas, int mahkumdurumu, int EngellilikOrani, char KanamaDurumu[15]){
	int oncelikpuani=yaspuani(yas)+Ismahkum(mahkumdurumu)+(EngellilikOrani/4)+KanamaDegeri(KanamaDurumu);
	return oncelikpuani;
}

int MuayeneSuresiHesapla(int yas, int EngellilikOrani, char KanamaDurumu[15]){
	int MuayeneSuresi=10;
	if (yas>=65)
		MuayeneSuresi+=15;
	if(KanamaDurumu=="agirKanama")
		MuayeneSuresi+=20;
	else if(KanamaDurumu=="kanama    ")
		MuayeneSuresi+=10;
	MuayeneSuresi+=EngellilikOrani/5;
	return MuayeneSuresi;	
}

void DakikaEkle(int dakika) {
    int saatn, dakika_saat, dakika_toplam;

    sscanf(muayenesaati, "%d.%d", &saatn, &dakika_saat);

    dakika_toplam = saatn * 60 + dakika_saat + dakika;
    saatn = dakika_toplam / 60;
    dakika_saat = dakika_toplam % 60;

    snprintf(muayenesaati, sizeof(muayenesaati), "%02d.%02d", saatn, dakika_saat);
}

void HastaMuayeneEt(struct Hasta *MuayeneHastasi) {
    strcpy(MuayeneHastasi->MuayeneSaati,muayenesaati);
    printf("\n%d. sýrada %d numaralý %s adlý hasta muayene olmuþtur.\n", muayenesirasi, MuayeneHastasi->HastaNo, MuayeneHastasi->HastaAdi);
    printf("Öncelik Puaný: %d   Muayene süresi: %d  Muayene Giriþ Saati:%s\n", MuayeneHastasi->Oncelik, MuayeneHastasi->MuayeneSuresi, muayenesaati);
    muayenesirasi++;
}

void OncelikSirala() {
    struct Hasta* gecici = NULL;
    struct Hasta* suanki = NULL;
    int degisiklik;

    if (KayitOlanlar == NULL) {
        printf("Hastalar kayýt edilmedi.\n");
        return;
    }

    do {
        degisiklik = 0;
        suanki = KayitOlanlar; //KayitOlanlar struct'ýný kaybetmemek için

        while (suanki->Next != gecici) {
            if (suanki->Oncelik < suanki->Next->Oncelik) {
                // Hasta düðümlerini yer deðiþtir
                struct Hasta* geciciTemp = suanki->Next;
                suanki->Next = suanki->Next->Next;
                geciciTemp->Next = suanki;

                if (suanki == KayitOlanlar) {
                    KayitOlanlar = geciciTemp;
                }
                else {
                    struct Hasta* onceki = KayitOlanlar;
                    while (onceki->Next != suanki) {
                        onceki = onceki->Next;
                    }
                    onceki->Next = geciciTemp;
                }

                degisiklik = 1;
            }
            else {
                suanki = suanki->Next;
            }
        }
        gecici = suanki;
    } while (degisiklik);
}


void HastaKayitEkle(struct Hasta* r) {
    strcpy(muayenesaati, "09.00");
    strcpy(saat,"08.00");
    struct Hasta *sonKayit;
    
    int i = 0;
    while (r->Next != NULL) {
    	//printf("  saat:%s  muayenesaati:%s \n",saat,muayenesaati);
        if (KayitOlanlar == NULL) {
            KayitOlanlar=r;
            sonKayit=r;
            strcpy(saat,r->HastaKayitSaati);
            r = r->Next;
        }
        else if(IsBuyuk(r->HastaKayitSaati,muayenesaati)){
        		i++;
        		HastaMuayeneEt(KayitOlanlar);
        		DakikaEkle(KayitOlanlar->MuayeneSuresi);
        		KayitOlanlar=KayitOlanlar->Next;
		}
		else{
			sonKayit=KayitOlanlar;
			while(sonKayit->Next!=NULL)
				sonKayit=sonKayit->Next;
			sonKayit->Next=r;
			strcpy(saat,r->HastaKayitSaati);
        	sonKayit=sonKayit->Next;
        	r = r->Next;
        	sonKayit->Next=NULL;
        	OncelikSirala();
			if (strcmp(saat, "16.30") == 0)
        		break;
        	
		}
    } 
    while(KayitOlanlar!=NULL){
    	i++;
        HastaMuayeneEt(KayitOlanlar);
        DakikaEkle(KayitOlanlar->MuayeneSuresi);
       	KayitOlanlar=KayitOlanlar->Next;
	}
	printf("\n\n TÜM HASTALAR BAÞARIYLA TEDAVÝ EDÝLDÝ...:)");
}

int main(int argc, char *argv[]) {
	setlocale(LC_ALL,"Turkish");
	Hastalariyukle();
	HastaNoEkle();
	//Yazdir(ilkHasta);
	
	printf("\n\n");
	HastaKayitEkle(ilkHasta);

	return 0;
	
}

