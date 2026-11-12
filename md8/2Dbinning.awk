#!/usr/bin/awk -f
#
#  takes two columns and prints the 2D binned probability distribution
BEGIN{
    colx=6; coly=7; nbins=30; tmin=100; if(tmax=="") tmax=1e6
}
!/#/&&$1>tmin&&$1<tmax{
    n++
    v1[n]=$(colx)
    v2[n]=$(coly)
}
END{
    max1=min1=v1[1]
    max2=min2=v2[1]
    for(i=1;i<=n;i++){
	if(v1[i]>max1) max1=v1[i];
	if(v1[i]<min1) min1=v1[i];
	if(v2[i]>max2) max2=v2[i];
	if(v2[i]<min2) min2=v2[i];
    }
    range1=max1-min1; delta1=range1/nbins
    range2=max2-min2; delta2=range2/nbins
    print "min1=",min1,", max1=",max1,", min2=",min2,", max2=",max2 > "/dev/stderr"
    for(i=0;i<=nbins;i++)
	for(j=0;j<=nbins;j++) p[i][j]="0.0"
    for(i=1;i<=n;i++){
	bin1=int((v1[i]-min1)/delta1)
	bin2=int((v2[i]-min2)/delta2)
	p[bin1][bin2]+=1
    }
    for(i=0;i<=nbins;i++){
	for(j=0;j<=nbins;j++) print (i+0.5)*delta1+min1,(j+0.5)*delta2+min2,p[i][j]/(n*delta1*delta2)
        print ""
    }
}
