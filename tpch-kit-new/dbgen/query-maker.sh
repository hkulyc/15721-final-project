export DSS_CONFIG=/Users/arvinwu/Documents/CMU-Senior-S2/15721/final/15721-final-project/tpch-kit/dbgen
export DSS_QUERY=$DSS_CONFIG/queries
export DSS_PATH=/Users/arvinwu/Documents/CMU-Senior-S2/15721/final/15721-final-project/tpch-kit/dbgen-output

nl=$'\n'

for ((i=1;i<=22;i++)); do
  ./qgen -v -c -d -s 1 ${i} | sed 's/limit -1;//' | perl -0777 -p -e 's/;\nlimit/\nlimit/'  | sed 's/day (3)/day/'  > ../queries/tpch-q${i}.sql
done