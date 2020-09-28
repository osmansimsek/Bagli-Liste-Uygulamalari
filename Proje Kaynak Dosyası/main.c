#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
struct komsusehirler
{
    int plakakodu1;
    struct komsusehirler *next;
};
struct sehir
{
    int plakakodu;
    char *sehirismi;
    char *bolge;
    int komsusayisi;
    struct komsusehirler *komsu;
    struct sehir *next;
    struct sehir *previous;
};
FILE *sehirler;
struct sehir *root=NULL;
int sehirsayisi=0;
void siraliyapiolustur(char kisim[]);
void komsulukolustur(char kisim[],struct sehir*iteral);
void menu();
void sehirekle(struct sehir *temp);
void sehircikar(struct sehir *temp);
void komsucikar(int plakakodu,struct sehir *temp);
void komsuekle(int plakakodu,struct sehir *iteral);
char* trim(char *kelime);
void bastir();
void dosyacek();
int main()
{
    printf("\n");
    printf("           Bagli Liste Uygulamasi\n\n\n");
    printf("islem 1: Sehir Veya Komsu Ekle\n");
    printf("islem 2: Sehir Ara\n");
    printf("islem 3: Sehir Veya Komsuluk Cikar\n");
    printf("islem 4: Bolgeye Gore Listele\n");
    printf("islem 5: Komsu Sayisina Gore Listele\n");
    printf("islem 6: Cikti.txt ye yazdir\n");
    printf("islem 7: Bonus\n");
    printf("islem 8: Cikis\n\n");
    menu();
    return 0;
}
void dosyacek()
{
    char kisim[1000];
    sehirler=fopen("sehirler.txt","r");
    if(sehirler==NULL)
    {
        exit(1);
    }
    else
    {
        while(fgets(kisim,1000,sehirler)!=NULL)
        {
            siraliyapiolustur(kisim);
        }
        fseek(sehirler,0,SEEK_SET);
        struct sehir *iter=root;
        while(iter!=NULL)
        {
            if(fgets(kisim,1000,sehirler)!=NULL)
            {
                komsulukolustur(kisim,iter);
            }
            iter=iter->next;
        }
    }
}
void siraliyapiolustur(char kisim[])
{
    struct sehir *temp;
    temp=malloc(sizeof(struct sehir));
    temp->next=NULL;
    temp->previous=NULL;
    int kelimesayac=1;
    char *ayrac = ",";
    char *kelime = strtok(kisim,ayrac);
    while(kelime!=NULL)
    {
        if(kelimesayac==1)
        {
            temp->plakakodu=atoi(kelime);
        }
        if(kelimesayac==2)
        {
            temp->sehirismi=malloc(sizeof(kelime)*10);
            strcpy(temp->sehirismi,kelime);
        }
        if(kelimesayac==3)
        {
            temp->bolge=malloc(sizeof(kelime)*10);
            strcpy(temp->bolge,kelime);
            temp->komsusayisi=0;
            sehirekle(temp);
        }
        kelime=strtok(NULL,ayrac);
        kelimesayac++;
    }
    sehirsayisi++;
}
void komsulukolustur(char kisim[],struct sehir *iteral)
{
    int kelimesayac=1;
    char *ayrac = ",";
    char *kelime = strtok(kisim,ayrac);
    while(kelime!=NULL)
    {
        if(kelimesayac>3)
        {
            struct sehir *gez=root;
            while(gez!=NULL)
            {
                if(strcmp(gez->sehirismi,trim(kelime))==0)
                {
                    komsuekle(gez->plakakodu,iteral);
                }
                gez=gez->next;
            }
        }
        kelime=strtok(NULL,ayrac);
        kelimesayac++;
    }
}
void komsuekle(int plakakodu, struct sehir *iteral)
{

    struct komsusehirler *temp=malloc(sizeof(struct komsusehirler));
    temp->plakakodu1=plakakodu;
    temp->next=NULL;
    if(iteral->komsusayisi==0 && iteral->komsu==NULL)
    {
        iteral->komsu=temp;
        iteral->komsusayisi++;
    }
    else if(iteral->komsusayisi==1 && plakakodu<iteral->komsu->plakakodu1)
    {
        temp->next=iteral->komsu;
        iteral->komsu=temp;
        iteral->komsusayisi++;
    }
    else if(iteral->komsusayisi==1 && plakakodu>iteral->komsu->plakakodu1)
    {
        iteral->komsu->next=temp;
        iteral->komsusayisi++;
    }
    else if(iteral->komsusayisi>1)
    {
        struct komsusehirler *r = iteral->komsu;
        while(r->next != NULL)
        {
            if(temp->plakakodu1<r->next->plakakodu1)
                break;
            r=r->next;
        }
        if(r->next==NULL && temp->plakakodu1>r->plakakodu1)
        {
            r->next=temp;
            iteral->komsusayisi++;
        }
        else if(temp->plakakodu1>r->plakakodu1)
        {
            temp->next=r->next;
            r->next=temp;
            iteral->komsusayisi++;
        }
        else if(temp->plakakodu1<r->plakakodu1)
        {
            temp->next=iteral->komsu;
            iteral->komsu=temp;
            iteral->komsusayisi++;
        }
    }
}
void komsucikar(int plakakodu, struct sehir *temp)
{

    struct komsusehirler *cikar;
    if(temp->komsusayisi==0 && temp->komsu==NULL)
    {
        printf("Sistemde Kayitli Komsu Yok. \n");
    }
    if(temp->komsusayisi==1 && temp->komsu->plakakodu1==plakakodu)
    {
        cikar=temp->komsu;
        temp->komsu=NULL;
        temp->komsusayisi--;
        free(cikar);
    }
    if(plakakodu==temp->komsu->plakakodu1)
    {
        cikar=temp->komsu;
        temp->komsu=temp->komsu->next;
        free(cikar);
        temp->komsusayisi--;
    }
    else if(temp->komsusayisi>1)
    {
        struct komsusehirler *gez = temp->komsu;
        while(gez!=NULL)
        {
            if(plakakodu==gez->next->plakakodu1)
                break;
            gez=gez->next;
        }
        if(gez->next->next==NULL && plakakodu==gez->next->plakakodu1)
        {
            cikar=gez->next;
            gez->next=NULL;
            free(cikar);
            temp->komsusayisi--;
        }
        else if(plakakodu==gez->next->plakakodu1 && gez->next->next!=NULL)
        {
            cikar=gez->next;
            gez->next=gez->next->next;
            free(cikar);
            temp->komsusayisi--;
        }
    }
}
void sehircikar(struct sehir *temp)
{
    struct sehir *cikar;
    if(sehirsayisi==0 && root==NULL)
    {
        printf("Sistemde Kayitli il Yok. \n");
    }
    if(sehirsayisi==1 && temp->plakakodu==root->plakakodu)
    {
        cikar=root;
        root=NULL;
        sehirsayisi--;
        free(cikar);
    }
    else if(sehirsayisi>1)
    {
        struct sehir *gez = root;
        while(gez->next!=NULL)
        {
            if(temp->plakakodu==gez->plakakodu)
                break;
            gez=gez->next;
        }
        if(gez->next==NULL && temp->plakakodu==gez->plakakodu)
        {
            cikar= gez;
            gez->previous->next=NULL;
            free(cikar);
            sehirsayisi--;
        }
        else if(gez->previous==NULL && temp->plakakodu==gez->plakakodu)
        {
            cikar=gez;
            root=gez->next;
            root->previous=NULL;
            free(cikar);
            sehirsayisi--;
        }
        else
        {
            cikar = gez;
            gez->previous->next=gez->next;
            gez->next->previous=gez->previous;
            free(gez);
        }
    }
}
void sehirekle(struct sehir *temp)
{
    temp->komsu=NULL;
    temp->next=NULL;
    temp->previous=NULL;

    if(sehirsayisi==0 && root==NULL)
    {
        root=malloc(sizeof(struct sehir));
        root=temp;
        root->next=NULL;
        root->previous=NULL;
    }
    else if(sehirsayisi==1 && temp->plakakodu<root->plakakodu)
    {
        temp->next=root;
        root->previous=temp;
        root=temp;
    }
    else if(sehirsayisi==1 && temp->plakakodu>root->plakakodu)
    {
        root->next=temp;
        temp->previous=root;
    }
    else
    {
        struct sehir *r=root;
        while(r->next!=NULL)
        {
            if(temp->plakakodu<r->next->plakakodu)
                break;
            r=r->next;
        }
        if(r->next==NULL && temp->plakakodu>r->plakakodu)
        {
            temp->next=NULL;
            r->next=temp;
            temp->previous=r;
        }
        else if(temp->plakakodu>r->plakakodu)
        {
            temp->next=r->next;
            temp->previous=r;
            r->next->previous=temp;
            r->next=temp;
        }
        else if(temp->plakakodu<r->plakakodu)
        {
            temp->next=root;
            root->previous=temp;
            root=temp;
        }
    }
}
char *trim(char *s)
{
    char *ptr;
    if (!s)
        return NULL;
    if (!*s)
        return s;
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
}

void menu()
{
    dosyacek();
    FILE *cikti;

    printf("1 den 8 e kadar bir islem seciniz\n");

    while(1)
    {
        char *komsuadal = malloc(sizeof(char)*200);
        char *komsuadal2 = malloc(sizeof(char)*200);
        struct sehir *temp=malloc(sizeof(struct sehir));
        temp->sehirismi=malloc(sizeof(char)*100);
        temp->bolge=malloc(sizeof(char)*100);
        struct sehir *gez=root;
        struct sehir *gezV2=root;
        int islem,komsuplakaal,secim;
        int kontrol=0;
        printf("Bir islem seciniz: ");
        scanf("%d",&islem);
        fflush(stdin);
        switch(islem)
        {

        case 1:

            printf("Sehir Eklemek icin - 1 | Komsuluk eklemek icin - 2: ");
            scanf("%d",&secim);
            fflush(stdin);
            if(secim!=1 && secim!=2)
            {
                printf("yanlis secim yaptiniz!!\n\n");
                break;
            }
            if(secim==1)
            {
                printf("Eklemek Istediginiz Sehrin Adini Giriniz: ");
                gets(temp->sehirismi);
                fflush(stdin);
                while(gez!=NULL)
                {
                    if(strcasecmp(trim(gez->sehirismi),trim(temp->sehirismi))==0)
                    {
                        kontrol=1;
                        printf("Ayni isme sahip iki sehir olamaz!!\n\n");
                        break;
                    }
                    gez=gez->next;
                }
                if (kontrol==1)
                    break;
                else
                {
                    gez=root;
                    kontrol=0;
                }
                printf("Eklemek Istediginiz Sehrin Plakasini Giriniz: ");
                scanf("%d",&temp->plakakodu);
                fflush(stdin);
                while(gez!=NULL)
                {
                    if(strcasecmp(trim(gez->sehirismi),trim(temp->sehirismi))!=0 && gez->plakakodu==temp->plakakodu)
                    {
                        kontrol=1;
                        printf("Ayni plakaya sahip iki sehir olamaz!!\n\n");
                        break;
                    }
                    gez=gez->next;
                }
                if (kontrol==1)
                    break;
                printf("Eklemek Istediginiz Sehrin Bolgesini Giriniz: ");
                gets(temp->bolge);
                fflush(stdin);
                if(kontrol==0)
                {
                    sehirekle(temp);
                    printf("Sehir basariyla eklendi.\n\n");
                    free(temp);
                }
            }

            if(secim==2)
            {
                printf("Komsuluk eklemek istediginiz sehrin adini giriniz: ");
                gets(komsuadal);
                fflush(stdin);
                while(gez!=NULL && strcasecmp(trim(gez->sehirismi),trim(komsuadal))!=0)
                {
                    gez=gez->next;
                }
                if(gez==NULL)
                {
                    printf("Komusuluk eklemek istediginiz sehir sistemde yoktur.\n\n");
                    free(komsuadal);
                    break;
                }
                else
                    gez=root;
                printf("Komsu olarak eklemek istediginiz sehrin adini giriniz: ");
                gets(komsuadal2);
                fflush(stdin);
                while(gez!=NULL)
                {
                    if(strcasecmp(komsuadal2,gez->sehirismi)==0)
                    {
                        komsuplakaal=gez->plakakodu;
                        break;
                    }
                    gez=gez->next;
                }
                if(gez==NULL)
                {
                    printf("Komsu olarak eklemek istediginiz sehir sistemde yoktur.\n\n");
                    free(komsuadal);
                    free(komsuadal2);
                    break;
                }
                else
                    gez=root;
                while(gez!=NULL)
                {

                    if(strcasecmp(trim(gez->sehirismi),trim(komsuadal))==0)
                    {
                        struct komsusehirler *V1 = gez->komsu;
                        while(V1!=NULL)
                        {
                            if(V1->plakakodu1==komsuplakaal)
                            {
                                kontrol=1;
                                break;
                            }
                            V1=V1->next;
                        }
                        free(V1);
                    }
                    gez=gez->next;
                }
                if(kontrol==1)
                {
                    printf("Komsu olarak eklemek istediginiz sehir zaten ekli!!\n\n");
                    free(komsuadal);
                    free(komsuadal2);
                    break;
                }
                else
                    gez=root;
                while(gez!=NULL)
                {
                    if(strcasecmp(trim(gez->sehirismi),trim(komsuadal))==0)
                    {
                        komsuekle(komsuplakaal,gez);
                        while(gezV2!=NULL)
                        {

                            if(gezV2->plakakodu==komsuplakaal)
                            {
                                komsuekle(gez->plakakodu,gezV2);
                                break;
                            }
                            gezV2=gezV2->next;
                        }
                        kontrol=2;
                        break;
                    }
                    gez=gez->next;
                }

                if(kontrol==2)
                    printf("Eklemek istediginiz komsuluk basariyla eklenmistir.\n\n");
            }
            break;

        case 2:

            printf("listeden sehir bul\n");
            printf("sehir ismine gore arat - 1 | plakaya gore arat - 2: ");
            scanf("%d",&secim);
            fflush(stdin);
            if(secim!=1 && secim!=2)
            {
                printf("yanlis secim yaptiniz!!\n\n");
                break;
            }
            if(secim==1)
            {
                char *isimarat = malloc(sizeof(char)*200);
                printf("Aratmak istediginiz sehrin ismini giriniz: ");
                gets(isimarat);
                fflush(stdin);
                while(gez!=NULL)
                {
                    if(strcasecmp(gez->sehirismi,isimarat)==0)
                    {
                        kontrol=1;
                        printf("\n");
                        printf("  SEHRIN BILGILERI\n\n");
                        printf("Sehir Adi: %s\n",gez->sehirismi);
                        printf("Plaka kodu: %d\n",gez->plakakodu);
                        printf("Bolgesi: %s\n",gez->bolge);
                        printf("Komsu Sayisi: %d\n\n",gez->komsusayisi);
                        printf("  KOMSU BILGILERI:\n\n");
                        struct komsusehirler *V2=gez->komsu;
                        while(V2!=NULL)
                        {
                            while(gezV2!=NULL)
                            {
                                if(V2->plakakodu1==gezV2->plakakodu)
                                {
                                    printf("Sehir Adi: %s\n",gezV2->sehirismi);
                                    printf("Plaka kodu: %d\n",gezV2->plakakodu);
                                    printf("Bolgesi: %s\n",gezV2->bolge);
                                    printf("Komsu Sayisi: %d\n\n",gezV2->komsusayisi);
                                    printf("-------------------\n");
                                }
                                gezV2=gezV2->next;
                            }
                            gezV2=root;
                            V2=V2->next;
                        }
                        free(V2);
                    }
                    gez=gez->next;
                }
            }
            if(secim==2)
            {
                int plakaarat;
                printf("Aratmak istediginiz plakayi giriniz: ");
                scanf("%d",&plakaarat);
                fflush(stdin);
                while(gez!=NULL)
                {
                    if(gez->plakakodu==plakaarat)
                    {
                        kontrol=1;
                        printf("\n");
                        printf("  SEHRIN BILGILERI\n\n");
                        printf("Sehir Adi: %s\n",gez->sehirismi);
                        printf("Plaka kodu: %d\n",gez->plakakodu);
                        printf("Bolgesi: %s\n",gez->bolge);
                        printf("Komsu Sayisi: %d\n\n",gez->komsusayisi);
                        printf("  KOMSU BILGILERI:\n\n");
                        struct komsusehirler *V2 = gez->komsu;
                        while(V2!=NULL)
                        {
                            while(gezV2!=NULL)
                            {
                                if(V2->plakakodu1==gezV2->plakakodu)
                                {
                                    printf("Sehir Adi: %s\n",gezV2->sehirismi);
                                    printf("Plaka kodu: %d\n",gezV2->plakakodu);
                                    printf("Bolgesi: %s\n",gezV2->bolge);
                                    printf("Komsu Sayisi: %d\n\n",gezV2->komsusayisi);
                                    printf("-------------------\n");
                                }
                                gezV2=gezV2->next;
                            }
                            gezV2=root;
                            V2=V2->next;
                        }
                        free(V2);
                    }
                    gez=gez->next;
                }
            }
            if(kontrol==0)
            {
                gez=root;
                printf("Aradiginiz sehir sistemde yok eklemek istermisiniz: \n");
                printf("evet - 1 | hayir - 2\n");
                int tercih;
                scanf("%d",&tercih);
                fflush(stdin);
                if(tercih!=1 && tercih!=2)
                {
                    printf("Yanlis tercih yaptiniz!!\n\n");
                    break;
                }
                if(tercih==1)
                {
                    printf("Eklemek Istediginiz Sehrin Adini Giriniz: ");
                    gets(temp->sehirismi);
                    fflush(stdin);
                    while(gez!=NULL)
                    {
                        if(strcasecmp(trim(gez->sehirismi),trim(temp->sehirismi))==0)
                        {
                            kontrol=1;
                            printf("Ayni isme sahip iki sehir olamaz!!\n\n");
                            break;
                        }
                        gez=gez->next;
                    }
                    if (kontrol==1)
                        break;
                    else
                        gez=root;
                    printf("Eklemek Istediginiz Sehrin Plakasini Giriniz: ");
                    scanf("%d",&temp->plakakodu);
                    fflush(stdin);
                    while(gez!=NULL)
                    {
                        if(strcasecmp(trim(gez->sehirismi),trim(temp->sehirismi))!=0 && gez->plakakodu==temp->plakakodu)
                        {
                            kontrol=1;
                            printf("Ayni plakaya sahip iki sehir olamaz!!\n\n");

                            break;
                        }
                        gez=gez->next;
                    }
                    if (kontrol==1)
                        break;
                    printf("Eklemek Istediginiz Sehrin Bolgesini Giriniz: ");
                    gets(temp->bolge);
                    fflush(stdin);
                    if(kontrol==0)
                    {
                        sehirekle(temp);
                        printf("Sehir basariyla eklendi.\n\n");
                        free(temp);
                    }
                }
                if(tercih==2)
                {
                    break;
                }
            }
            break;

        case 3:

            printf("Sehir cikarmak icin - 1 | Komsuluk cikarmak icin - 2: ");
            scanf("%d",&secim);
            fflush(stdin);
            if(secim!=1 && secim!=2)
            {
                printf("yanlis secim yaptiniz!!\n\n");
                break;
            }
            if(secim==1)
            {
                printf("Cikarmak Istediginiz Sehrin Adini Giriniz: ");
                temp->plakakodu=0;
                gets(temp->sehirismi);
                fflush(stdin);
                while(gez!=NULL)
                {
                    if(strcasecmp(gez->sehirismi,temp->sehirismi)==0)
                    {
                        temp->plakakodu=gez->plakakodu;
                    }
                    gez=gez->next;
                }
                gez=root;
                while(gez!=NULL)
                {
                    if(gez->plakakodu==temp->plakakodu && strcasecmp(trim(gez->sehirismi),trim(temp->sehirismi))==0)
                    {
                        kontrol=1;
                        sehircikar(gez);
                        struct komsusehirler *V3 = gez->komsu;
                        while(V3!=NULL)
                        {
                            gezV2 = root;
                            while(gezV2!=NULL)
                            {
                                if(gezV2->plakakodu==V3->plakakodu1)
                                {
                                    komsucikar(temp->plakakodu,gezV2);
                                }
                                gezV2=gezV2->next;
                            }
                            gezV2=root;
                            V3=V3->next;
                        }
                        printf("Sehir basariyla cikarildi.\n\n");
                        free(temp);
                        break;
                    }
                    gez=gez->next;
                }
                if(kontrol==0)
                {
                    printf("Boyle bir sehir zaten mevcut degil!!\n\n");
                    free(temp);
                }
            }
            if(secim==2)
            {
                printf("Komsuluk cikarmak istediginiz sehrin adini giriniz: ");
                gets(komsuadal);
                fflush(stdin);
                while(gez!=NULL && strcasecmp(trim(gez->sehirismi),trim(komsuadal))!=0)
                {
                    gez=gez->next;
                }
                if(gez==NULL)
                {
                    printf("Komusuluk cikarmak istediginiz sehir sistemde yoktur.\n\n");
                    break;
                }
                printf("Komsuluk cikartma islemini nasil yapmak istersiniz (1 - komsu sehir plakasi , 2 - komsu sehir adi): ");
                int islemsec=0;
                scanf("%d",&islemsec);
                fflush(stdin);
                if(islemsec==1)
                {
                    printf("Komsu olarak cikarmak istediginiz sehrin plakasini giriniz: ");
                    scanf("%d",&komsuplakaal);
                    fflush(stdin);
                    gez=root;
                    while(gez!=NULL)
                    {
                        if(gez->plakakodu==komsuplakaal){

                            strcpy(komsuadal2,gez->sehirismi);
                            break;
                        }
                        gez=gez->next;
                    }
                    if(gez==NULL)
                    {
                        printf("Komsu cikarmak istediginiz sehir sistemde yoktur.\n\n");
                        break;
                    }
                    else
                        gez=root;
                }
                if(islemsec==2)
                {
                    printf("Komsu olarak cikarmak istediginiz sehrin adini giriniz: ");
                    gets(komsuadal2);
                    fflush(stdin);
                    gez=root;
                    while(gez!=NULL)
                    {
                        if(strcasecmp(gez->sehirismi,komsuadal2)==0)
                        {
                            komsuplakaal=gez->plakakodu;
                            break;
                        }
                        gez=gez->next;
                    }
                    if(gez==NULL)
                    {
                        printf("Komsu cikarmak istediginiz sehir sistemde yoktur.\n\n");
                        break;
                    }
                    else
                        gez=root;
                }
                while(gez!=NULL)
                {
                    if(strcasecmp(trim(gez->sehirismi),trim(komsuadal))==0)
                    {
                        struct komsusehirler *V3 = gez->komsu;
                        while(V3!=NULL)
                        {
                            if(V3->plakakodu1==komsuplakaal)
                            {
                                kontrol=1;
                                break;
                            }
                            V3=V3->next;
                        }
                        free(V3);
                    }
                    gez=gez->next;
                }
                if(kontrol==0)
                {
                    printf("Komsu olarak cikarmak istediginiz sehir zaten ekli degil!!\n\n");
                    free(komsuadal2);
                    free(komsuadal);
                    break;
                }
                gez=root;
                while(gez!=NULL)
                {
                    if(strcasecmp(trim(gez->sehirismi),trim(komsuadal))==0)
                    {
                        komsucikar(komsuplakaal,gez);
                        while(gezV2!=NULL)
                        {
                            if(gezV2->plakakodu==komsuplakaal)
                            {
                                komsucikar(gez->plakakodu,gezV2);
                                break;
                            }
                            gezV2=gezV2->next;
                        }
                        kontrol=2;
                        break;
                    }
                    gez=gez->next;
                }
                if(kontrol==2)
                    printf("Cikarmak istediginiz komsuluk basariyla cikarilmistir.\n\n");

            }
            break;

        case 4:
            printf("1. Bolge: Marmara(MA)\n");
            printf("2. Bolge: Karadeniz(KA)\n");
            printf("3. Bolge: Ege(EG)\n");
            printf("4. Bolge: Akdeniz(AK)\n");
            printf("5. Bolge: Ic Anadolu(IA)\n");
            printf("6. Bolge: Dogu Anadolu(DA)\n");
            printf("7. Bolge: Guney Dogu Anadolu(GA)\n");
            printf("Bu bolgelerden listelemek istediginizin seciniz: \n");
            char*bolgeara=malloc(sizeof(char)*10);
            while(1)
            {
                int secim1;
                scanf("%d",&secim1);
                switch(secim1)
                {
                    secim1=0;
                    scanf("%d",&secim1);
                    fflush(stdin);

                case 1:
                    strcpy(bolgeara,"MA");
                    break;

                case 2:
                    strcpy(bolgeara,"KA");
                    break;

                case 3:
                    strcpy(bolgeara,"EG");
                    break;

                case 4:
                    strcpy(bolgeara,"AK");
                    break;

                case 5:
                    strcpy(bolgeara,"IA");
                    break;

                case 6:
                    strcpy(bolgeara,"DA");
                    break;

                case 7:
                    strcpy(bolgeara,"GA");
                    break;

                default:
                    printf("Yanlis secim yaptiniz lutfen tekrar seciniz!! \n");
                    break;

                }
                if(secim1>=1 && secim1<=7)
                    break;
            }

            while(gez!=NULL)
            {
                if(strcmp(gez->bolge,bolgeara)==0)
                {
                    printf("Sehir Adi: %s\n",gez->sehirismi);
                    printf("Plaka kodu: %d\n",gez->plakakodu);
                    printf("Komsu Sayisi: %d\n",gez->komsusayisi);
                    printf("-------------------\n");
                }
                gez=gez->next;
            }
            break;

        case 5:
            printf("Komsu sayisina gore listelemek istediginiz sayiyi giriniz: ");
            int plakaara;
            scanf("%d",&plakaara);
            fflush(stdin);
            printf("Girdiginiz sayinin ve altindakileri sehirleri listelemek icin 1.\n");
            printf("Girdiginiz sayinin ve ustundekileri sehirleri listelemek icin 2.\n");
            printf("Girdiginiz sayiya esit olan sehirleri listelemek icin 3.\n");
            int secenek;
            scanf("%d",&secenek);
            fflush(stdin);
            if(secenek!=1 && secenek!=2 && secenek!=3)
            {
                printf("Yanlis secim yaptiniz!!");
                break;
            }
            while(gez!=NULL)
            {
                if(secenek==1)
                {
                    if(gez->komsusayisi>plakaara)
                    {
                        printf("Sehir Adi: %s\n",gez->sehirismi);
                        printf("Plaka kodu: %d\n",gez->plakakodu);
                        printf("Bolge: %s\n",gez->bolge);
                        printf("Komsu Sayisi: %d\n",gez->komsusayisi);
                        struct komsusehirler *V5 = gez->komsu;
                        while(V5!=NULL)
                        {
                            printf("%d ",V5->plakakodu1);
                            V5=V5->next;
                        }
                        printf("\n");
                        printf("-------------------\n");
                        free(V5);
                    }
                }
                if(secenek==2)
                {
                    if(gez->komsusayisi<plakaara)
                    {
                        printf("Sehir Adi: %s\n",gez->sehirismi);
                        printf("Plaka kodu: %d\n",gez->plakakodu);
                        printf("Bolge: %s\n",gez->bolge);
                        printf("Komsu Sayisi: %d\n",gez->komsusayisi);
                        struct komsusehirler *V5 = gez->komsu;
                        while(V5!=NULL)
                        {
                            printf("%d ",V5->plakakodu1);
                            V5=V5->next;
                        }
                        printf("\n");
                        printf("-------------------\n");
                        free(V5);
                    }
                }
                if(secenek==3)
                {

                    if(gez->komsusayisi==plakaara)
                    {
                        printf("Sehir Adi: %s\n",gez->sehirismi);
                        printf("Plaka kodu: %d\n",gez->plakakodu);
                        printf("Bolge: %s\n",gez->bolge);
                        printf("Komsu Sayisi: %d\n",gez->komsusayisi);
                        struct komsusehirler *V5 = gez->komsu;
                        while(V5!=NULL)
                        {
                            printf("%d ",V5->plakakodu1);
                            V5=V5->next;
                        }
                        printf("\n");
                        printf("-------------------\n");
                        free(V5);
                    }
                }
                gez=gez->next;
            }
            break;

        case 6:

            cikti=fopen("cikti.txt","w");
            while(gez!=NULL)
            {
                fprintf(cikti,"Sehir Adi: %s\n",gez->sehirismi);
                fprintf(cikti,"Plaka kodu: %d ------------> ",gez->plakakodu);
                struct komsusehirler *V6=gez->komsu;
                while(V6!=NULL)
                {
                    fprintf(cikti,"%d ",V6->plakakodu1);
                    V6=V6->next;
                }
                fprintf(cikti,"\n");
                fprintf(cikti,"Bolge: %s\n\n",gez->bolge);
                fprintf(cikti,"       ||    \n\n");
                free(V6);
                gez=gez->next;
            }
            fclose(cikti);
            break;

        case 7:

            while(1)
            {
                printf("belirli bir sayi araligindaki komsulari bulmak icin iki sayi giriniz: \n");
                int sayactut=0;
                int sayi1,sayi2;
                printf("Sayi 1: ");
                scanf("%d",&sayi1);
                fflush(stdin);
                printf("Sayi 2: ");
                scanf("%d",&sayi2);
                fflush(stdin);
                if(sayi1==sayi2)
                {
                    printf("sayilari esit girdiniz lutfen aralikli olmasina dikkat edin!!\n");
                    continue;
                }
                if(sayi1>sayi2)
                {
                    int tut = sayi1;
                    sayi1=sayi2;
                    sayi2=tut;
                }
                printf("Kac sehirin ortak komsularina bakmak istersin: ");
                int sayi3;
                scanf("%d",&sayi3);
                fflush(stdin);
                int i;
                printf("Ortak komsulari aramak icin sistemde olan %d sehir giriniz: \n",sayi3);
                char *sehir[sayi3];
                for(i=0; i<sayi3; i++)
                {
                    printf("sehir %d: ",i+1);
                    sehir[i]=malloc(sizeof(char)*100);
                    fgets(sehir[i],100,stdin);
                    while(gez!=NULL)
                    {
                        if(strcasecmp(trim(gez->sehirismi),trim(sehir[i]))==0)
                        {
                            kontrol=5;
                        }
                        gez=gez->next;
                    }
                    gez=root;
                    if(kontrol==0)
                    {
                        printf("Girdiginiz %d. sehir sistemde yoktur lutfen tekrar giriniz!!\n",i+1);
                        kontrol=0;
                        break;
                    }
                }
                if(kontrol==0)
                    continue;

                while(gez!=NULL)
                {

                    if(gez->komsusayisi>sayi1 && gez->komsusayisi<sayi2)
                    {
                        gezV2=root;
                        while(gezV2!=NULL)
                        {
                            if(strcasecmp(trim(gezV2->sehirismi),trim(sehir[0]))==0)
                            {
                                struct komsusehirler *gezV2komsu=gezV2->komsu;
                                while(gezV2komsu!=NULL)
                                {
                                    if(gezV2komsu->plakakodu1==gez->plakakodu)
                                    {
                                        int j;
                                        int kontrol7_2=0;
                                        int kontrol7_3=(sayi3-1)*5;
                                        for(j=1; j<sayi3; j++)
                                        {
                                            struct sehir *gezV3=root;
                                            while(gezV3!=NULL)
                                            {
                                                if(strcasecmp(trim(gezV3->sehirismi),trim(sehir[j]))==0)
                                                {
                                                    struct komsusehirler *gezV3komsu=gezV3->komsu;
                                                    while(gezV3komsu!=NULL)
                                                    {
                                                        if(gezV3komsu->plakakodu1==gez->plakakodu)
                                                        {
                                                            kontrol7_2+=5;
                                                        }
                                                        gezV3komsu=gezV3komsu->next;
                                                    }
                                                    free(gezV3komsu);
                                                }
                                                gezV3=gezV3->next;
                                            }
                                            gezV3=root;
                                        }
                                        if(kontrol7_2==kontrol7_3)
                                        {
                                            printf("\n");
                                            printf("\"%s\"\n",gez->sehirismi);
                                            printf("------------\n");
                                            sayactut++;
                                        }
                                    }
                                    gezV2komsu=gezV2komsu->next;
                                }
                                free(gezV2komsu);
                            }
                            gezV2=gezV2->next;
                        }
                        gezV2=root;
                    }
                    gez=gez->next;
                }
                if(sayactut==0)
                {
                    printf("Herhangi bir ortak komsuluk bulunamadi.\n\n");
                    break;
                }
                if(sayactut>0)
                {
                    printf("Girilen kriterlere gore bulunan ortak komsuluk sayisi: %d\n\n",sayactut);
                    break;
                }
            }
            break;

        case 8:
            exit(1);
            break;

        default:
            printf("Yanlis secim yaptiniz lutfen tekrar seciniz!! \n\n");
            break;
        }
    }
}
