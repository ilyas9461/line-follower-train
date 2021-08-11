
#define FIS_TYPE_INT unsigned int16 //float
#define FIS_TYPE_F float
#define FIS_RESOLUSION 101

#define max(x,y)  ((x > y) ? x : y) 
#define min(x,y)  ((x < y) ? x : y)

FIS_TYPE_INT mf_in_sb_trap_a[2] = { 0, 130 };    //  trapezoid a
FIS_TYPE_INT mf_in_sb_trap_b[2] = { 0, 700 };    //  trapezoid b
FIS_TYPE_INT mf_in_sb_trap_c[2] = { 130, 800 };    //  trapezoid b
FIS_TYPE_INT mf_in_sb_trap_d[2] = { 700, 800 };    //  trapezoid b

FIS_TYPE_INT mf_out_pos_ua[17] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
FIS_TYPE_INT mf_out_pos_ub[17] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
FIS_TYPE_INT mf_oup_pos_uc[17] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

int8 kurallar[25] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

FIS_TYPE_F bulanik_giris_ir1[2]={0.0,0.0};    // 8 adet it sensör
FIS_TYPE_F bulanik_giris_ir2[2]={0.0,0.0};
FIS_TYPE_F bulanik_giris_ir3[2]={0.0,0.0};
FIS_TYPE_F bulanik_giris_ir4[2]={0.0,0.0};
FIS_TYPE_F bulanik_giris_ir5[2]={0.0,0.0};
FIS_TYPE_F bulanik_giris_ir6[2]={0.0,0.0};
FIS_TYPE_F bulanik_giris_ir7[2]={0.0,0.0};
FIS_TYPE_F bulanik_giris_ir8[2]={0.0,0.0};

FIS_TYPE_F bulanik_girisler[2][8]={ bulanik_giris_ir1,bulanik_giris_ir2,bulanik_giris_ir3, bulanik_giris_ir4,
                                    bulanik_giris_ir5, bulanik_giris_ir6,bulanik_giris_ir7,bulanik_giris_ir8 };


FIS_TYPE_F dizi_blOperator[17];       // kural sayýsý kadar
FIS_TYPE_F dizi_imp[17];              // gerektirme kural sayýsý kadar


FIS_TYPE_F min_uygula_dizi(FIS_TYPE_F *d){
    int x=0;
    FIS_TYPE_F mn=d[0];
    for(x=1;x<8;x++){
        mn=min(mn,d[x]);
    }
    return mn;
}
FIS_TYPE_F ucgenUyelikFonk(FIS_TYPE_INT x, FIS_TYPE_INT a, FIS_TYPE_INT b, FIS_TYPE_INT c)
{
	FIS_TYPE_F t1 = (x - a) / (b - a);
	FIS_TYPE_F t2 = (c - x) / (c - b);
	if ((a == b) && (b == c)) return (FIS_TYPE_F)(x == a);
	if (a == b) return (FIS_TYPE_F)(t2*(b <= x)*(x <= c));
	if (b == c) return (FIS_TYPE_F)(t1*(a <= x)*(x <= b));
	t1 = min(t1, t2);
	return (FIS_TYPE_F)max(t1, 0);
}

FIS_TYPE_F trapMf(FIS_TYPE_INT x, FIS_TYPE_INT a, FIS_TYPE_INT b, FIS_TYPE_INt c, FIS_TYPE_INT d)
{
	//FIS_TYPE a = params[0], b = params[1], c = params[2], d = params[3];
	FIS_TYPE_F y1 = 0.0, y2 = 0.0;
//
//	if (a>b) {
//		PRINTF("a = %f, b = %f, c = %f, d = %f\n", a, b, c, d);
//		fisError("Illegal parameters in fisTrapezoidMf() --> a > b");
//	}
//        if (b>c)
//         {
//                PRINTF("a = %f, b = %f, c = %f, d = %f\n", a, b, c, d);      
//                fisError("Illegal parameters in fisTrapezoidMf() --> b > c");
//         }
//	if (c>d) {
//		PRINTF("a = %f, b = %f, c = %f, d = %f\n", a, b, c, d);
//		fisError("Illegal parameters in fisTrapezoidMf() --> c > d");
//	}

	if (b <= x)
		y1 = 1.0;
	else if (x < a)
		y1 = 0.0;
	else if (a != b)
		y1 = (x-a)/(b-a);

	if (x <= c)
		y2 = 1.0;
	else if (d < x)
		y2 = 0.0;
	else if (c != d)
		y2 = (d-x)/(d-c);

	return(min(y1, y2));
	/*
	if (a == b && c == d)
		return((b<=x)*(x<=c));
	if (a == b)
		return(MIN(1, (d-x)/(d-c))*(b<=x)*(x<=d));
	if (c == d)
		return(MIN((x-a)/(b-a), 1)*(a<=x)*(x<=c));
	return(MAX(MIN(MIN((x-a)/(b-a), 1), (d-x)/(d-c)), 0));
	*/
}

void bulaniklastir(FIS_TYPE_INT *keskin_g){
	int x = 0,y=0;
 
	for (x = 0; x < 2; x++)     //giriþ üyelik fonk kadar
	    bulanik_giris_ir1[x]=trapMf(keskin_g[0], mf_in_sb_trap_a[x], mf_in_sb_trap_b[x], mf_in_sb_trap_c[x],mf_in_sb_trap_d[x]);
    for (x = 0; x < 2; x++)     //giriþ üyelik fonk kadar
	    bulanik_giris_ir2[x]=trapMf(keskin_g[1], mf_in_sb_trap_a[x], mf_in_sb_trap_b[x], mf_in_sb_trap_c[x],mf_in_sb_trap_d[x]);
    for (x = 0; x < 2; x++)     //giriþ üyelik fonk kadar
	    bulanik_giris_ir3[x]=trapMf(keskin_g[2], mf_in_sb_trap_a[x], mf_in_sb_trap_b[x], mf_in_sb_trap_c[x],mf_in_sb_trap_d[x]);
    for (x = 0; x < 2; x++)     //giriþ üyelik fonk kadar
	    bulanik_giris_ir4[x]=trapMf(keskin_g[3], mf_in_sb_trap_a[x], mf_in_sb_trap_b[x], mf_in_sb_trap_c[x],mf_in_sb_trap_d[x]);
    for (x = 0; x < 2; x++)     //giriþ üyelik fonk kadar
	    bulanik_giris_ir5[x]=trapMf(keskin_g[4], mf_in_sb_trap_a[x], mf_in_sb_trap_b[x], mf_in_sb_trap_c[x],mf_in_sb_trap_d[x]);
    for (x = 0; x < 2; x++)     //giriþ üyelik fonk kadar
	    bulanik_giris_ir6[x]=trapMf(keskin_g[5], mf_in_sb_trap_a[x], mf_in_sb_trap_b[x], mf_in_sb_trap_c[x],mf_in_sb_trap_d[x]);
    for (x = 0; x < 2; x++)     //giriþ üyelik fonk kadar
	    bulanik_giris_ir7[x]=trapMf(keskin_g[6], mf_in_sb_trap_a[x], mf_in_sb_trap_b[x], mf_in_sb_trap_c[x],mf_in_sb_trap_d[x]);
    for (x = 0; x < 2; x++)     //giriþ üyelik fonk kadar
	    bulanik_giris_ir8[x]=trapMf(keskin_g[7], mf_in_sb_trap_a[x], mf_in_sb_trap_b[x], mf_in_sb_trap_c[x],mf_in_sb_trap_d[x]);
	
}
void bulanikOperatorUygula(FIS_TYPE_F* fuzzy_girisler){
    int i=0;
	for (int x = 0; x <8; x++){
		for (int y = 0; y < 2; y++){
			dizi_blOperator[i++] = min(fuzzy_girisler[x][y],fuzzy_girisler[x]); 
			
            //Kurallarýn if kýsýmlarýna bulanýk operator and-->min
			//uygulandý ve diziye alýndý...
		}
	}
}