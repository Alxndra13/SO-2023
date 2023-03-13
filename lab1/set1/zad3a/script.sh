cd results
for element in results*.txt
do
    echo "$element" >> ../report.txt
    echo "-------------------" >> ../report.txt
    cat "$element" >> ../report.txt
    echo -e "\n" >> ../report.txt
done
cd ..