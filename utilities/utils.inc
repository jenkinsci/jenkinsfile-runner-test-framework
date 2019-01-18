# file with miscellaneous utils

# $1: origin
# $2: string to remove from origin
remove_string() {

	if [ "$#" -gt 1 ]
	then
		origin="$1"
		origin_length=${#origin}

		to_remove="$2"
		length_to_remove=${#to_remove}

		result=$(echo "$origin" | cut -b $length_to_remove-$origin_length)

		echo "$result"
		return 0
    else
    	echo "Error. Missing parameters. Use remove_string origin string_to_remove"
    	return 1
    fi

}
