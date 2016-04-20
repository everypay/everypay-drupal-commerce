<?php

/* vim: set expandtab tabstop=2 shiftwidth=2 softtabstop=2: */

class Everypay
{
  const LIVE_URI = 'https://api.everypay.gr';

  const SANDBOX_URI = 'https://sandbox-api.everypay.gr';

  private $payment_method = array();

  public function __construct($payment_method)
  {
    $this->payment_method = $payment_method;
  }

  public function getMode()
  {
    return $this->getSettings('mode');
  }

  public function isSandbox()
  {
    return (int) ($this->getMode() == COMMERCE_EVERYPAY_MODE_SANDBOX);
  }

  public function getKey($key)
  {
    $mode = $this->getMode();

    return $this->payment_method['settings'][$mode][$key];
  }

  public function pay($amount, $token)
  {
    $data = array(
      'amount' => $amount,
      'token' => $token,
    );

    return $this->sendRequest($data);
  }

  public function createTransaction($order, $instance_id, $amount, $currency_code, $status, $response)
  {
    $transaction = commerce_payment_transaction_new('commerce_everypay', $order->order_id);
    $transaction->instance_id = $instance_id;
    $transaction->amount = $amount;
    $transaction->currency_code = $currency_code;
    $transaction->status = $status;
    $transaction->remote_status = isset($response['status']) ? $response['status'] : $responce['error']['status'];
    $transaction->message = isset($response['error']) ? $response['error']['message'] : $response['status'];
    $transaction->remote_id = isset($response['token']) ? $response['token'] : null;

    commerce_payment_transaction_save($transaction);
  }

  public function renderButton($amount, $currency_code)
  {
    $params = array(
      'public_key' => $this->getKey('public_key'),
      'amount' => $amount,
      'currency_code' => $currency_code,
      'sandbox' => $this->isSandbox(),
      'label' => t('Submit card data'),
    );

    return $this->renderTemplate($params);
  }

  private function renderTemplate($params)
  {
    extract($params);

    $level = ob_get_level();
    ob_start();
    ob_implicit_flush(0);
    try {
      include dirname(__FILE__) . '/commerce_everypay.template.inc';
    } catch (Exception $e) {
      while (ob_get_level() > $level) {
        ob_end_clean();
      }
      throw $e;
    }

    return ob_get_clean();
  }

  private function getUri()
  {
    return $this->getMode() == COMMERCE_EVERYPAY_MODE_SANDBOX
      ? self::SANDBOX_URI
      : self::LIVE_URI;
  }

  private function getSettings($key, $default = null)
  {
    $settings = $this->payment_method['settings'];

    return array_key_exists($key, $settings)
      ? $settings[$key]
      : $default;
  }

  private function sendRequest($data, $path = 'payments')
  {
    $uri = $this->getUri();
    $secret_key = $this->getKey('secret_key');

    $fields_string = http_build_query($data);
    //cURL Request
    $ch = curl_init();
    //set the url, number of POST vars, POST data
    curl_setopt($ch, CURLOPT_URL, $uri . '/' . $path);
    curl_setopt($ch, CURLOPT_USERPWD, $secret_key.':');
    curl_setopt($ch, CURLOPT_TIMEOUT, 60);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $fields_string);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, true);
    curl_setopt($ch, CURLOPT_USERAGENT, 'Everypay Drupal Commerce Plugin/0.1.0 (+https://www.everypay.gr)');

    $result = curl_exec($ch);
    $http_status = curl_getinfo($ch, CURLINFO_HTTP_CODE);

    $response_array = json_decode($result, true);

    return $response_array;
  }
}
