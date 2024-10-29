echo "--APA Project Benchmark--"

make

k=1
for instance in i/*; do
	echo $instance >> ./optimized_results.txt

	echo "Running $instance"
	echo "Instance $k of 16" 

	./apa ${instance} | awk "{print $1}" >> ./optimized_results.txt
	k=$(($k + 1))
done

echo "-" >> ./optimized_results.txt