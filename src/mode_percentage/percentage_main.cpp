#include "percentage_data.h"


void percentage_main(std::vector < std::string > & argv)
{
    perc_data P;

    boost::program_options::options_description opt_basic ("\x1B[32mBasics\33[0m");
	opt_basic.add_options()
		("help", "Produces option description")
		("seed", boost::program_options::value< unsigned int >()->default_value(123456), "Random number seed. Useful to replicate runs.");
    
    boost::program_options::options_description opt_files ("\x1B[32mI/O\33[0m");
	opt_files.add_options()
		("bed", boost::program_options::value< std::string >(), "Phenotypes in BED format.")
		("out", boost::program_options::value< std::string >(), "Output file.");

    boost::program_options::options_description opt_parallel ("\x1B[32mParallelization\33[0m");
	opt_parallel.add_options()
		("missingness", boost::program_options::value< double >(), "expression missingness threshold. Remove if more/equal than threshold.")
        ("percentage", "Write percentage of missing and present expression across samples.")
        ("includePhen", boost::program_options::value< std::string>(), "Include phenotypes from bed file. Phenotypes should be in bed format.");
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
    std::cout << "\n" << "\x1B[32;1m" << "BED PHENOTYPE INCLUSION" << "\033[0m" << endl;
    if(P.options.count("help")) {
        std::cout << P.option_descriptions << std::endl;
        exit(0);
    }

    //-----------------
	// 4. COMMON CHECKS
	//-----------------
    if (!P.options.count("bed")) std::cout <<"Phenotype data needs to be specified with --bed [file.bed]" << std::endl;
    if (!P.options.count("out")) std::cout << "Output needs to be specified with --out [file.out]" << std::endl; 
    int nMode = P.options.count("missingness") + P.options.count("percentage") + P.options.count("includePhen");
    if (nMode != 1) std::cout << "Please, specify only one of these options [--missingness, --percentage, --includePhen]" << std::endl;
    std::string outFile = P.options["out"].as<std::string>();
    //---------
	// 5. MODES
	//---------

    //MODE 1: Missingness 
    if (P.options.count("missingness")){
        P.mode = PERC_FILTER;
        P.perc_threshold = P.options["missingness"].as<double>();
        std::cout << " * Removing phenotypes with ≥ " << std::to_string(P.perc_threshold) << " missingness" << std::endl;
        P.writeData(P.options["bed"].as<std::string>(),outFile);
    }

    //MODE 2: Percentage
    if (P.options.count("percentage")){
        P.mode = PERC_WRITE;
        std::cout << " * Writing percentage of present and missing expression accross samples" << std::endl;
        P.writeData(P.options["bed"].as<std::string>(),outFile);
    }

    if (P.options.count("includePhen")){
        P.mode = PERC_NONE;
        std::cout << "  * Including phenotypes in provided bed file" << std::endl;
        P.readPhenotypesToInclude(P.options["excludePhen"].as<std::string>());
        P.excludePhenotypes(P.options["bed"].as<std::string>(),outFile);
    }
    //-------------
	// RUN ANALYSIS
	//-------------

    

}


