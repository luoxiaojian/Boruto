
fout1=open('preemption.log', 'w+')
fout2=open('migration.log', 'w+')

for i in range(17):
    ut=(i+4)*5
    fname='./result/u_'+str(ut)
    fin=open(fname, 'r')
    sumP=0
    sumM=0
    count=0
    for line in fin.readlines():
        data=line.strip().split('\t')
        preemption=float(data[0])
        migration=float(data[1])
        sumP+=preemption
        sumM+=migration
        count=count+1
    sumP=sumP/count
    sumM=sumM/count
    fout1.write(str(ut)+'\t'+str(sumP)+'\n')
    fout2.write(str(ut)+'\t'+str(sumM)+'\n')
    fin.close()
    fout1.flush()
    fout2.flush()

fout1.close()
fout2.close()


