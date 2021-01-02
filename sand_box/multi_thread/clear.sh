source_file_array=(`find . -name  '*.cpp'`) 
declare binary_file_array

#${#array[@]}) return to the array size
# 从右向左截取第一个string后的字符串 ${varible%string*}
#

for index in $(seq 0 ${#source_file_array[@]}); 
do
    binary_file_array[index]=${source_file_array[index]%.*}
    rm -rf ${binary_file_array[index]}
done

