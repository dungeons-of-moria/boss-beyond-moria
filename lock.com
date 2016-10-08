$! keep this file around.  you need it to make '@build lock'
$! and '@build unlock' work.
$!
$ write sys$output " "
$ write sys$output " "
$ write sys$output "                  The BOSS is taking a vacation."
$ write sys$output "                Let him rest and enjoy himself without"
$ write sys$output "                worrying about you ruining his"
$ write sys$output "                stay in pago-pago."
$ write sys$output " "
$ write sys$output " "
$ write sys$output "                  No, actually changes are being made.."
$ write sys$output "                BOSS should be back online in a day or so."
$ exit
