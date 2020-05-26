# HPC-Proj-4

This project used three different methods of synchonization for the trapazoid method (to compute approximate area under curve) using omp. The three methods used are busy waiting, mutex, and semaphore. 



The second part of the project involved creating producers and consumers. The producers read from files and add data to an array. The consumers then read 'sentences' from the array and tokenize them and print the tokens. The generate text file is used to create the text file for the producers to read so it must run once to create the files. The generator creates files of differing lengths with mostly unique text for each word. There is some overlap in words once you get to double digit nuumbers of files, but the purpose is simply for debugging and I wasn't thinking that far ahead at the time. 


To compile "$ gcc <file_name>.c -o <file_name> -fopenmp" 

The function is hardcoded so to run use: "$ ./<file_name> <num_threads>"
