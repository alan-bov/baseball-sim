import argparse
from helpers.save_functions import save_pitcher, save_batter
from helpers.data_collectors import Pitcher, Batter, pitcher_lookup, batter_lookup

def main():
    parser = argparse.ArgumentParser(description='Get pitcher info')
    parser.add_argument('first_name', type=str, help='First name of the pitcher')
    parser.add_argument('last_name', type=str, help='Last name of the pitcher')
    parser.add_argument('year', type=str, help='Year for the season we are gathering')
    parser.add_argument('role', type=str, help='pitcher or batter')

    args = parser.parse_args()

    if args.role == 'pitcher':
        pitcher = Pitcher(args.first_name, args.last_name, args.year)
        pitcher = pitcher_lookup(pitcher)
        save_pitcher(pitcher)
    elif args.role == 'batter':
        batter = Batter(args.first_name, args.last_name, args.year)
        batter = batter_lookup(batter)
        save_batter(batter)
    else:
        print(f"Invalid role: '{args.role}'. Expected 'pitcher' or 'batter'.")
        parser.print_help()
        exit(1)

if __name__ == "__main__":
    main()
