{
  global: pam_sm_*; # explicitly list symbols to be exported
  local: *;         # hide everything else
};
