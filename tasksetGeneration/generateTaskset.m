function [exe, pri, res, hp]=generateTaskset(tn, pn, gr, tmin, tmax, ut)
exe=ones(1, tn);
pri=ones(1, tn);

execute=ones(tn, 1);

lb=log(tmin);
hb=log(tmax+gr);
width=hb-lb;
periods=floor(exp(rand(tn, 1)*width+lb)/gr)*gr;

[x, v]=randfixedsum(tn, 1, pn*ut, 0, 1);

x=sort(x,'descend');

for i=1:floor(tn/2)
	tmp=x(i)*periods(i);
	execute(i)=floor(tmp);
end

for i=floor(tn/2):tn
	tmp=x(i)*periods(i);
	execute(i)=ceil(tmp);
end

hp=1;
times=ones(tn-1, 1);
for i=1:tn-1
	hp=lcm(hp, periods(i));
end

if(hp>200000)
	res=0;
	return ;
end

if(hp*pn*ut~=fix(hp*pn*ut))
	res=0;
	return ;
end

for i=1:tn-1
	times(i)=hp/periods(i);
end

sum=0;
for i=1:tn-1
	sum=sum+times(i)*execute(i);
end

remains=hp*pn*ut-sum;
if(remains<=0)
	res=0;
	return
end

fac=gcd(hp, remains);
fenzi=remains/fac;
fenmu=hp/fac;

if(fenmu>1000000)
	res=0;
	return ;
end

if mod(fenmu, gr)~=0
	gcdgcd=gcd(fenmu, gr);
	fenmu=fenmu*gr/gcdgcd;
	fenzi=fenzi*gr/gcdgcd;
end

execute(tn)=fenzi;
periods(tn)=fenmu;

for i=1:tn
	if(execute(i)>=periods(i))
		res=0;
		return ;
	end
	if(execute(i)==0)
		res=0;
		return ;
	end
end

for i=1:tn
	pri(i)=periods(i);
end
for i=1:tn
	exe(i)=execute(i);
end

res=1;
return
