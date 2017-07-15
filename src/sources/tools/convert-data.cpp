// Copyright 2015, 2016, 2017 Ingo Steinwart
//
// This file is part of liquidSVM.
//
// liquidSVM is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as 
// published by the Free Software Foundation, either version 3 of the 
// License, or (at your option) any later version.
//
// liquidSVM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License
// along with liquidSVM. If not, see <http://www.gnu.org/licenses/>.



#include "../shared/system_support/os_specifics.h"
#include "../shared/basic_functions/flush_print.h"
#include "../shared/basic_functions/random_subsets.h"

#include "../shared/basic_types/dataset_info.h"
#include "../shared/system_support/timing.h"
#include "../shared/command_line/command_line_parser.h"


//**********************************************************************************************************************************



unsigned const ERROR_clp_tco_s = 110;


//**********************************************************************************************************************************


class Tcommand_line_parser_convert: public Tcommand_line_parser
{
	public:
		Tcommand_line_parser_convert();
		void parse();
		
		
		bool permute_data;
		unsigned subset_size;
		vector <Tsample_file_format> file_formats;

	protected:
		void exit_with_help();
		void display_help(unsigned error_code);
};


//**********************************************************************************************************************************

Tcommand_line_parser_convert::Tcommand_line_parser_convert()
{
	permute_data = false;
	subset_size = 0;
	
	command_name = "convert";
};

//**********************************************************************************************************************************

void Tcommand_line_parser_convert::parse()
{
	check_parameter_list_size();
	for(current_position=1; current_position<parameter_list_size; current_position++)
		if (Tcommand_line_parser::parse("-d-h-r") == false)
		{
			if(parameter_list[current_position][0] != '-') 
				break;
			if (string(parameter_list[current_position]).size() > 2)
				Tcommand_line_parser::exit_with_help(ERROR_clp_gen_unknown_option);
			
			switch(parameter_list[current_position][1])
			{
				case 's':
						subset_size = get_next_number(ERROR_clp_tco_s, 0);
						permute_data = get_next_bool(ERROR_clp_tco_s);
						break;
				default:
					Tcommand_line_parser::exit_with_help(ERROR_clp_gen_unknown_option);
			}			
		}
		
	file_formats.push_back(get_next_data_file_format(ERROR_clp_gen_missing_data_file_name));
	file_formats.push_back(get_next_data_file_format(ERROR_clp_gen_missing_data_file_name));
	
	while (current_position < parameter_list_size)
	{
		file_formats.push_back(get_next_data_file_format(ERROR_clp_gen_missing_data_file_name));
		current_position++;
	}
};


//**********************************************************************************************************************************

void Tcommand_line_parser_convert::exit_with_help()
{
	flush_info(INFO_SILENCE,
	"\n\nconvert [options] <data_file_output> <data_file1_input> [<data_file2_input> ...]\n"
	"\nWrites the data set contained in <data_fileX_input> into"
	"\nthe file <data_file_output>. A type conversion is performed"
	"\naccording to the file extensions and the file format specifers.\n"
	"\nAllowed extensions:\n"
		"<data_filex_input>:  .csv and .lsv\n"
		"<data_file_output>:  .csv and .lsv\n");
	display_help_file_formats();
	
	if (full_help == false)
		flush_info(INFO_SILENCE, "\nOptions:");

	display_help(ERROR_clp_gen_d);
	display_help(ERROR_clp_gen_h);
	display_help(ERROR_clp_gen_r);
	display_help(ERROR_clp_tco_s);
	
	flush_info(INFO_SILENCE,"\n\n");
	copyright();
	flush_exit(ERROR_SILENT, "");
};


//**********************************************************************************************************************************


void Tcommand_line_parser_convert::display_help(unsigned error_code)
{
	Tcommand_line_parser::display_help(error_code);
	
	if (error_code == ERROR_clp_tco_s)
	{
		display_separator("-s <subset_size> <permute_flag>");
		flush_info(INFO_1, 
		"Saves a subset of size <subset_size>, if this values is > 0. If <permute_flag>\n"
		"is set, the subset is randomly chosen, otherwise, the first entries are taken.\n");

		display_ranges();
		flush_info(INFO_1, "<subset_size>:   unsigned integer\n");
		flush_info(INFO_1, "<permute_flag>:  bool\n");
	}
}
	


//**********************************************************************************************************************************
//**********************************************************************************************************************************
//**********************************************************************************************************************************



int main(int argc, char **argv)
{
	Tcommand_line_parser_convert command_line_parser;

	Tdataset data_set;
	Tdataset data_set_tmp;
	Tdataset data_set_final;
	Tdataset_info data_set_info;

	unsigned i;
	unsigned subset_size;
	vector <unsigned> subset_info;
	
	double read_time_tmp;
	double read_time;
	double write_time;
	double full_time;


// Read command line
	
	read_time = 0.0;
	full_time = get_wall_time_difference();
	

	command_line_parser.setup(argc, argv);
	command_line_parser.parse();


// Load datasets

	data_set.enforce_ownership();
	for (i=1; i<command_line_parser.file_formats.size(); i++)
	{
		read_time_tmp = get_process_time_difference();
		data_set_tmp.read_from_file(command_line_parser.file_formats[i]);
		read_time = read_time + get_process_time_difference(read_time_tmp);
		data_set.push_back(data_set_tmp);
	}


// Pick subset

	if (command_line_parser.subset_size == 0)
		subset_size = data_set.size();
	else
		subset_size = command_line_parser.subset_size;
	
	if (command_line_parser.permute_data == false)
		subset_info = id_permutation(subset_size);
	else
		subset_info = random_subset(id_permutation(data_set.size()), subset_size, command_line_parser.get_random_seed());

	data_set.create_subset(data_set_final, subset_info);

	
// Write dataset to file

	write_time = get_process_time_difference();
	data_set_final.write_to_file(command_line_parser.file_formats[0]);
	write_time = get_process_time_difference(write_time);


// Clean up

 	full_time = get_wall_time_difference(full_time);

	flush_info(INFO_1,"\n\n%4.2f seconds used to run convert.", full_time);
	flush_info(INFO_1,"\n%4.2f seconds used for read from file operations.", read_time);
	flush_info(INFO_1,"\n%4.2f seconds used for write to file operations.", write_time);

	command_line_parser.copyright();
	
	flush_info(INFO_1,"\n\n");
}


