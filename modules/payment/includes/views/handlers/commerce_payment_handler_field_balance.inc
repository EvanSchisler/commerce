<?php
//$Id$

/**
 * Field handler to be able to show the balance order with currency.
 */
class commerce_payment_handler_field_balance extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    $this->additional_fields['order_id'] = 'order_id';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['display_format'] = array('default' => 'formatted');

    return $options;
  }

  /**
   * Provide the currency format option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['display_format'] = array(
      '#type' => 'select',
      '#title' => t('Display format'),
      '#options' => array(
        'formatted' => t('Currency formatted amount'),
        'raw' => t('Raw amount'),
      ),
      '#default_value' => $this->options['display_format'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    switch ($this->options['display_format']) {
      case 'formatted':
        // Get the order object and the balance.
        $order = commerce_order_load($values->{$this->aliases['order_id']});
        $balance = commerce_payment_order_balance($order);
        // Format the amount with currency.
        return commerce_currency_format($balance['amount'], $balance['currency_code']);
      case 'raw':
        // Get the order object and the balance.
        $order = commerce_order_load($values->{$this->aliases['order_id']});
        $balance = commerce_payment_order_balance($order);
        // Then load the full currency object.
        $currency = commerce_currency_load();
        // Format the price as a number.
        return number_format(commerce_currency_round($balance['amount'], $currency), $currency->decimals);
    }
  }
}