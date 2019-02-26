## Stanisław Grams <sgrams@secpass.org>
## 2019-02-19 16:40
message_file=$1
if [ -z "$1" ]
then
  echo "no argument given"
  exit 1
fi

PS3="Select an option:"
options=("read messages" "write a message" "quit")
select opt in "${options[@]}"
do
  case $opt in
    "read messages")
      cat $message_file
      ;;
    "write a message")
      read input
      echo $input >> $message_file
      ;;
    "quit")
      exit 0
      ;;
    *) echo "invalid option $REPLY" ;;
  esac
done
