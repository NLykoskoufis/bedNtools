#include "vcf2bed_data.h"

void vcf2bed_main(std::vector < std::string > & argv)
{
    Vcf2bed P;

    boost::program_options::options_description opt_basic ("\x1B[32mBasics\33[0m");
	opt_basic.add_options()
		("help", "Produces option description");
    
    boost::program_options::options_description opt_files ("\x1B[32mI/O\33[0m");
	opt_files.add_options()
		("vcf", boost::program_options::value<  std::string >(), "VCF file")
		("out", boost::program_options::value<  std::string >(), "Output file in BED format.");

    boost::program_options::options_description opt_parallel ("\x1B[32mParallelization\33[0m");
	opt_parallel.add_options()
		("threads", boost::program_options::value< int >()->default_value(1), "Number of threads to use.");

    
    P.option_descriptions.add(opt_basic).add(opt_files).add(opt_parallel);

    //-------------------
	// 2. PARSE OPTIONS
	//-------------------
	try {
		boost::program_options::store(boost::program_options::command_line_parser(argv).options(P.option_descriptions).run(), P.options);
		boost::program_options::notify(P.options);
	} catch ( const boost::program_options::error& e ) {
		std::cerr << "Error parsing [null] command line :" << string(e.what()) << std::endl;
		exit(0);
	}

    //---------------------
	// 3. PRINT HELP/HEADER
	//---------------------
    std::cout << "\n" << "\x1B[32;1m" << "CONVERT VCF TO BED FILE" << "\033[0m" << endl;
    if(P.options.count("help")) {
        std::cout << P.option_descriptions << std::endl;
        exit(0);
    }

    //-----------------
	// 4. COMMON CHECKS
	//-----------------
    if (!P.options.count("vcf")) std::cout <<"Phenotype data needs to be specified with --bed [file.bed]" << std::endl;
    if (!P.options.count("out")) std::cout << "Output needs to be specified with --out [file.out]" << std::endl; 

    std::string outFile = P.options["out"].as< std::string>();
    


    //-------------
	// RUN ANALYSIS
	//-------------

    // SET THREADS 
    P.nthreads = P.options["threads"].as< int>();
    if (P.nthreads > 1) std::cout << " * Threads set to " << P.nthreads << std::endl;
    P.readGenotypes(P.options["vcf"].as< std::string>(),outFile);


}