file = open('queries/tpch-q2.sql',mode='r')
 
# read all lines at once
all_of_it = file.read()
 
# close the file
file.close()

print(all_of_it)
print (repr(all_of_it))