echo "--APA Project Benchmark--"

make

k=1
for instance in instances/*; do
	echo "Running $instance"
	echo "Instance $k of 16" 

	./apa ${instance} | awk "{print $1}" >> ./results.txt
	k=$(($k + 1))
done

echo "-" >> ./results.txt