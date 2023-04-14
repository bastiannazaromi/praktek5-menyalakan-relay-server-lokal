<?php

defined('BASEPATH') or exit('No direct script access allowed');

class Data extends CI_Controller
{
    public function getRelay()
    {
        $relay = $this->db->get('relay')->row();

        echo $relay->status;
    }
}

/* End of file Data.php */
